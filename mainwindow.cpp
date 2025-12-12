#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLibrary>
#include <QDebug>
//#include "windows.h"
#include <QPainter>

#include "hidapi.h"
#include <QDebug>
#include <QPen>
#include <QPoint>
#include <QPainter>
//#include <QPaintEvent>
#include <QScreen>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QMessageBox>
#include <QPainterPath>
typedef void (*CharReco_CharRecoInitFunction)();
typedef void(*CharReco_CharRecoUninitFunction)();
typedef void(*CharReco_CharRecoClearFunction)();
typedef int(*CharReco_StrokeEndFunction)();
typedef  bool (*CharReco_StrokingFunction)(POINT point);
typedef void (*CharRecoSvr_CharRecoInitFunction)();
typedef void (*CharReco_CharRecoGetParaFunction)(short m_pPoint_fromApp[MAX_POINT_NUM * 2 + 2],  // 坐标点缓冲区（为汉字结束标志预留一个点）
int &m_iPoint_fromApp,                   // 坐标点数
stStroke  m_pStroke_fromApp[MAX_STROKE_NUM],        // 笔画索引指针
int &m_iStroke_fromApp                 // 笔画数
);

CharReco_StrokeEndFunction CharReco_StrokeEnd=0;
CharReco_StrokingFunction CharReco_Stroking=0;
CharReco_CharRecoInitFunction CharReco_CharRecoInit=0;
CharReco_CharRecoUninitFunction CharReco_CharRecoUninit=0;
CharReco_CharRecoClearFunction CharReco_CharRecoClear=0;
CharReco_CharRecoGetParaFunction CharRecoGetPara=0;


typedef void (*CharRecoSvr_CharRecoSvrInitFunction)();
typedef void(*CharRecoSvr_CharRecoSvrUninitFunction)();
typedef  bool (*CharRecoSvr_RecognizeFunction)(short m_pPoint[MAX_POINT_NUM * 2 + 2],int &m_iPoint,  stStroke m_pStroke[MAX_STROKE_NUM],	int &m_iStroke,unsigned short m_pCharater[MAX_CANDIDATE_NUM + 1], bool bMatchMulti,/*LONG m_pDistance[MAX_CANDIDATE_NUM + 1],*/ unsigned int uMatchRange);

CharRecoSvr_RecognizeFunction CharRecoSvrRecognize=0;
CharRecoSvr_CharRecoSvrInitFunction CharRecoSvrInit=0;
CharRecoSvr_CharRecoSvrUninitFunction CharRecoSvrUninit=0;


void MainWindow::showMessage() {
        // 当点击右键菜单中的 "Show Message" 时，显示一条信息框
  //     QMessageBox::information(this, "Tray Icon", "This is a message from the tray icon.");

    delete penUpTimer;
    hid_exit();
       qDebug()<<"1";
     worker->bEndReadThread=false;
     qDebug()<<"2";
     CharReco_CharRecoUninit();
     CharRecoSvrUninit();



        qDebug()<<"3";
        QThread::sleep(1);
         workerThread->terminate();
       workerThread->wait();
        qDebug()<<"4";
        delete worker;
        delete workerThread;

   qDebug()<<"5";

//       worker->bEndReadThread=false;
       QCoreApplication::quit();
       qDebug()<<"6";


   exit(0);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置窗口背景透明
    setAttribute(Qt::WA_TranslucentBackground);
    // 可选：隐藏窗口边框
    setWindowFlags(Qt::FramelessWindowHint);
    // 可选：设置窗口的透明度（0.0 为完全透明，1.0 为完全不透明）
    setWindowOpacity(0.8);  // 半透明窗口

//    pix = QPixmap(this->size());

//    pix.fill(Qt::transparent);
    // 初始化 QTimer
        penUpTimer = new QTimer(this);
        penUpTimer->setInterval(2000);  // 设置定时器为3秒
        connect(penUpTimer, &QTimer::timeout, this, &MainWindow::clearCanvas);  // 超时时调用 clearCanvas 函数
        isPenUp = false;


    m_bBeginStroke = true;
    m_bWritting = false;
//    m_uDotNumber = 0;

    QLibrary lib("CharReco.dll");
    if (lib.load())
    {

        CharReco_CharRecoInit=(CharReco_CharRecoInitFunction)lib.resolve("CharRecoInit");
        CharReco_CharRecoUninit=(CharReco_CharRecoUninitFunction)lib.resolve("CharRecoUninit");
        CharReco_CharRecoClear=(CharReco_CharRecoClearFunction)lib.resolve("CharRecoClear");
        CharReco_StrokeEnd=(CharReco_StrokeEndFunction)lib.resolve("CharRecoStrokeEnd");
        CharReco_Stroking=(CharReco_StrokingFunction)lib.resolve("CharRecoStroking");
        CharRecoGetPara = (CharReco_CharRecoGetParaFunction)lib.resolve("CharRecoGetPara");
        if ((!CharReco_CharRecoInit)||(!CharReco_CharRecoUninit)||(!CharReco_CharRecoClear)||(!CharReco_StrokeEnd)||(!CharReco_Stroking)||(!CharRecoGetPara))
        {
            qDebug()<<"failed";
        }
        else
        {
            qDebug()<<"LoadDll OK";
            CharReco_CharRecoInit();
            //              CharReco_StrokeEnd();
            //             POINT pt;
            //             pt.x=1;
            //             pt.y=2;
            //             CharReco_Stroking(pt);
            CharReco_CharRecoClear();
            //             CharReco_CharRecoUninit();



        }
        //     lib.unload();
    }
    else
    {
        qDebug()<<"failed";
    }



    QLibrary libSvr("CharRecoSvr.dll");
    if (libSvr.load())
    {
        CharRecoSvrRecognize = (CharRecoSvr_RecognizeFunction)libSvr.resolve("CharRecoRecognize");
        CharRecoSvrInit = (CharRecoSvr_CharRecoSvrInitFunction)libSvr.resolve("CharRecoSvrInit");
        CharRecoSvrUninit = (CharRecoSvr_CharRecoSvrUninitFunction)libSvr.resolve("CharRecoSvrUninit");
        if ((!CharRecoSvrRecognize)||(!CharRecoSvrInit)||(!CharRecoSvrUninit))
        {
            qDebug()<<"Fail load CharRecoSvr.dll";
        }
        else
        {
            qDebug()<<"ok load CharRecoSvr.dll";
            CharRecoSvrInit();
        }
    }
    else
    {
        qDebug()<<"libSvr.load(CharRecoSvr) fail";
    }




    // 创建托盘图标
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon("d:\\Users\\21880\\Documents\\WeChat Files\\wxid_d4p1so1nholf22\\FileStorage\\File\\2025-01\\12.ico")); // 设置托盘图标的路径

    // 设置托盘图标的提示信息
    trayIcon->setToolTip("Threads Application");

    // 创建右键菜单
    QMenu *trayMenu = new QMenu(this);
    QAction *showAction = new QAction("Show Message", this);
    QAction *quitAction = new QAction("Quit", this);

    // 添加菜单项
    trayMenu->addAction(showAction);
    trayMenu->addAction(quitAction);

    // 设置托盘图标的右键菜单
    trayIcon->setContextMenu(trayMenu);

    // 连接菜单项的信号到槽
    connect(showAction, &QAction::triggered, this, &MainWindow::showMessage);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    // 启动托盘图标
    trayIcon->show();
    bFirstPress=false;
    // 初始化工作线程和 Worker
    workerThread = new QThread();
    worker = new WorkerThread();
    worker->moveToThread(workerThread);

    // 信号和槽连接
    connect(worker, &WorkerThread::resultReady, this, &MainWindow::handleResult);
 //  QMetaObject::invokeMethod(worker, "doWork", Qt::QueuedConnection);
   // connect(ui->startButton, &QPushButton::clicked, this, &threads::startTask);
    // 获取所有屏幕对象
       QList<QScreen *> screens = QApplication::screens();

       // 获取鼠标位置
       QPoint cursorPos = QCursor::pos();

       // 遍历所有屏幕，检查鼠标是否位于该屏幕
       foreach (QScreen *screen, screens)
       {
    QSize screenSize = screen->size();//获取当前屏幕的分辨率

            qDebug() << "Current Screen Resolution: "
                     << screenSize.width() << "x" << screenSize.height();// 输出屏幕分辨率
           width=screenSize.width();
           height=screenSize.height();
}
     MainWindow::startTask();
     workerThread->start();// 启动线程

}

#if 0
int MainWindow::GetDataByGetMethodFromServer(const QString &strUrl,int nHTTP_TIME,QJsonObject &strRetResult)
{
          return 0;
    //    qDebug("MainWindow::GetDataByGetMethodFromServer");
//    if (!bIsOnline)
//    {
//        qDebug()<<"is not online exit!";
//        return -1;
//    }

#if 0
    if (nHTTP_TIME > 100)
    {
        QNetworkAccessManager manager;
        QNetworkRequest request;
        //  request.setUrl(QUrl("http://qt-project.org"));
        //   request.setRawHeader("User-Agent", "MyOwnBrowser 1.0");

//        if (bUseNetworkProxy)
//        {
//            QNetworkProxy proxy;
//            proxy.setType(QNetworkProxy::HttpProxy);
//            // proxy.setHostName("192.168.1.103");
//            //  proxy.setPort(8080);
//            if (!strNetworkProxy_IP.isEmpty())
//            {
//                proxy.setHostName(strNetworkProxy_IP);
//            }
//            if (iNetworkProxy_Port!=-1)
//            {
//                proxy.setPort(iNetworkProxy_Port);
//            }

//            if (!strNetworkProxy_UserName.isEmpty())
//            {
//                proxy.setUser(strNetworkProxy_UserName);
//            }
//            if (!strNetworkProxy_Password.isEmpty())
//            {
//                proxy.setPassword(strNetworkProxy_Password);
//            }
//            manager.setProxy(proxy);
//        }

        request.setUrl(strUrl);
        request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
        QNetworkReply *pReply = manager.get(request);

        //      qDebug()<<"GetDataByGetMethodFromServer nHTTP_TIME>10:"<<nHTTP_TIME;
        QTimer timer;
        timer.setInterval(nHTTP_TIME);  // 设置超时时间 30 秒
        timer.setSingleShot(true);  // 单次触发
        QEventLoop loop;
        connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
        connect(pReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

        timer.start();
        loop.exec();  // 启动事件循环
        if (timer.isActive())
        {  // 处理响应

            timer.stop();

            if (pReply->error() != QNetworkReply::NoError) {

                // 错误处理

                //                  qDebug() << "Error String : " << pReply->errorString();
                return pReply->error();

            } else {

                QVariant variant = pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

                int nStatusCode = variant.toInt();

                // 根据状态码做进一步数据处理
                //                  qDebug() << "Status Code : " << nStatusCode;

                if (nStatusCode == 200)
                {
                    qDebug()<<"nStatusCode == 200";
                    QByteArray bytes = pReply->readAll();
                    //
                //    qDebug()<<bytes;
                    QJsonObject json_object = QJsonDocument::fromJson(bytes).object();
                    //
                    qDebug()<<"result:"<<json_object.value("result").toString();

                    strRetResult=json_object;
                }
                else
                {
                    qDebug()<<"nStatusCode != 200";
                }

                return nStatusCode;

            }

        } else {  // 处理超时


            disconnect(pReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

            pReply->abort();

            pReply->deleteLater();

            qDebug() << "GetDataByGetMethodFromServer Timeout";
            return -1;//timeout

        }


    }
    else
    {
        //       qDebug()<<"GetDataByGetMethodFromServer nHTTP_TIME < 10:"<<nHTTP_TIME;
        //        connect(pReply, SIGNAL(finished()), this, SLOT(slot_requestFinished_NotRead()));
        //        connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(slot_requestFinished_NotRead(QNetworkReply *)));



        //   QNetworkReply* m_Reply;



        //   QUrl url("http://m238352d31.wicp.vip:19004/klaiwebapi?0306798de2e3eea5a7fb0414c244986a1787cf3b88d1303a3a7735e01c93b1cf317");

        QUrl url(strUrl);

        qDebug()<<"GetDataByGetMethodFromServer nHTTP_TIME < 10 1";

        /*m_pNetworkReply = */m_pNetworkManager->get(QNetworkRequest(url));

        //       qDebug()<<"GetDataByGetMethodFromServer nHTTP_TIME < 10 2";


        //        connect(m_pNetworkManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(slot_requestFinished_NotRead(QNetworkReply *)));

        return 0;

    }

#endif
}
#endif
void MainWindow::DrawScreen(int nX, int nY, int nZ)
{

//    qDebug()<<"Enter DrawScreen";
    //CPoint pt = CPoint(nX, nY);
    QPoint pt = QPoint(nX, nY);

    if (pt.x()==0 && pt.y()==0)
    {
        if (m_bBeginStroke)
        {
            qDebug()<<"m_bBeginStroke is true and return";
            return;
        }


        qDebug("Will Call OnPenUp");
        // 提笔
        OnPenUp(pt);



        m_bBeginStroke = true;
        m_bWritting = false;
    }
    else if (m_bBeginStroke)
    {
        //		ClientToScreen(&pt);
        //if (pt == m_ptOld)	return;

        // 		char chBuf[100];
        // 		sprintf(chBuf,"x:%d,y:%d",pt.x,pt.y);
        // 		OutputDebugStringA(chBuf);

        // 落笔
        qDebug()<<"Will Call OnPenDown";
        OnPenDown(pt);

        m_bBeginStroke = false;
        m_bWritting = true;
    }
    else
    {

        //	ClientToScreen(&pt);
        //if (pt == m_ptOld)	return;

        // 		char chBuf[100];
        // 		sprintf(chBuf,"x:%d,y:%d",pt.x,pt.y);
        // 		OutputDebugStringA(chBuf);
        // 行笔
        qDebug()<<"Will Call OnPenMove";
        OnPenMove(pt, 0);
        //	OutputDebugStringA("OnPenMove");


    }

    //	m_ptOld = pt;


}

void MainWindow::OnPenDown(QPoint pt)//落笔
{

#if 1
    // 终止定时线程
    // 	if (m_pTimer)
    // 	{
    // 		KillTimer(NULL, m_pTimer);
    // 		m_pTimer = NULL;
    // 	}

    // 收集点阵
    POINT point;
    point.x=pt.x();
    point.y=pt.y();
    CharReco_Stroking(point);
//    m_uDotNumber ++;

    //	m_aPt.Add(pt);
    //	m_ptOldAverage = pt;

#endif

}

void MainWindow::OnPenMove(QPoint pt, unsigned int nZ)//笔移动
{

#if 1
    // 丢弃杂点
    //	if (abs(pt.x - m_ptOld.x) > MAX_DOT_X_OFFSET || abs(pt.y - m_ptOld.y) > MAX_DOT_Y_OFFSET)	return;

    // 收集点阵
    POINT point;
    point.x=pt.x();
    point.y=pt.y();
    CharReco_Stroking(point);
//    m_uDotNumber ++;

    //	m_aPt.Add(pt);

    //	CPoint ptOld = m_ptOldAverage;

#endif

}

void MainWindow::OnPenUp(QPoint pt)//起笔
{

#if 1
    // 沿原轨迹补上几个点模拟笔锋
    // 	if (m_bStrokeExtend && m_nLastZ==NULL && m_aPt.GetSize()>=5)
    // 	{
    // 	 		for (int t=0; t<3; t++)
    // 	 		{
    // 	 			int nX = ((m_aPt[m_aPt.GetSize()-1].x - m_aPt[m_aPt.GetSize()-2].x) + (m_aPt[m_aPt.GetSize()-2].x - m_aPt[m_aPt.GetSize()-3].x)) / 2;
    // 	 			int nY = ((m_aPt[m_aPt.GetSize()-1].y - m_aPt[m_aPt.GetSize()-2].y) + (m_aPt[m_aPt.GetSize()-2].y - m_aPt[m_aPt.GetSize()-3].y)) / 2;
    // 				OnPenMove( CPoint(m_aPt[m_aPt.GetSize()-1].x + nX * 2, m_aPt[m_aPt.GetSize()-1].y + nY * 2), m_nLastZ );
    // 			}
    // 	 	}

    CharReco_StrokeEnd();

    //	m_aPt.RemoveAll();

    // 提笔延迟收尾定时器
    // 	g_pThis = this;
    // 	m_pTimer = SetTimer(NULL, (UINT_PTR)this, m_uStrokeInterval, TimerProc);

#endif

}

void MainWindow:: paintEvent(QPaintEvent *)
{
//    QPainter pp(&pix);
//    pp.drawLine(lastPoint,endPoint);
//    lastPoint = endPoint;

//    QPainter painter(this);
//    painter.drawPixmap(0,0,pix);

    QPainter maopainter(this);
    maopainter.drawPixmap(0, 0, canvas);

//    QPainter handpainter(this);
//    QPen pen;
//    pen.setColor(Qt::black);   // 设置线条颜色
//    pen.setWidth(3);          // 设置线条宽度
//    handpainter.setPen(pen);
//    // 绘制曲线路径
//    if (!path.isEmpty()) {
//        handpainter.drawPath(path);  // 绘制曲线
//    }

}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        prevPoint = lastPoint;
        isDrawing = true;
        isLiftingPen = false; // 开始书写，重置提笔标记
        drawPenStroke(lastPoint, lastPoint);
    }
    // 如果是鼠标左键按下
    if(event->button() == Qt::LeftButton)
    {
        qDebug()<<"Press x:"<<event->x()<<"y:"<<event->y();
        lastPoint = event->pos();

        DrawScreen(event->x(),event->y(),0);
    }
    // 如果是鼠标右键按下
    else if(event->button() == Qt::RightButton)
    {

       QPixmap *clearPix = new QPixmap(size());
       clearPix->fill(Qt::transparent);

       pix = *clearPix;

       path = QPainterPath(); // 通过临时对象重新赋值

//        setAttribute(Qt::WA_TranslucentBackground);
       update();//清空写的字，并填充成白板，要改/

//       qDebug()<<"m_uDotNumber:"<<m_uDotNumber;

//       if (m_uDotNumber<5)
//       {
//           m_uDotNumber=0;
//                return;
//       }

//       m_uDotNumber=0;
//m_uDotNumber没有功能，是做测试的时候用的

        CharRecoGetPara(m_pPoint,m_iPoint,m_pStroke,m_iStroke);
        qDebug()<<"App m_iPoint:"<<m_iPoint<<"m_iStroke:"<<m_iStroke;

        qDebug()<<"App m_iPoint last:"<<m_pPoint[m_iPoint]<<m_pPoint[m_iPoint-1];

//        QString strm_pPoint = QString::fromLocal8Bit((char*)m_pPoint);

//        qDebug()<<"m_pPoint:1"<< strm_pPoint;

//        QString strPoint;
//        for (int i=0;i<m_iPoint*2;i++)
//        {
//            strPoint+= QString::number ( m_pPoint[i],  10);
//            if (i<m_iPoint*2-1)
//            {
//                strPoint+=";";
//            }
//        }
//        qDebug()<<"App strPoint:"<< strPoint;
//也是测试用的
//        QString strStroke;
//        for (int i=0;i<m_iStroke;i++)
//        {
//            strStroke+= QString::number ( m_pStroke[i].iBegin,  10)+"_"+QString::number ( m_pStroke[i].iEnd,  10)
//                    +"_"+QString::number ( m_pStroke[i].rect.left,  10)
//                    +"_"+QString::number ( m_pStroke[i].rect.top,  10)
//                    +"_"+QString::number ( m_pStroke[i].rect.right,  10)
//                    +"_"+QString::number ( m_pStroke[i].rect.bottom,  10);
//        if (i<m_iStroke-1)
//                    strStroke+=";";
//        }
//        qDebug()<<"App strStroke:"<< strStroke;
//同测试用
#if 0
        QString strSendDataValue=strPoint+"&"+QString::number ( m_iPoint*2,  10)+"&"+strStroke+"&"+QString::number ( m_iStroke,  10)+"&"+"0"+"&"+"480";
    //    QString strSendDataCryptData0 = *paesTools->Encrypt(&strSendDataValue);
    //    QString strSendDataCryptData;
    //    strSendDataCryptData.sprintf("%03d",strSendDataValue.length());
    //    strSendDataCryptData=strSendDataCryptData+strSendDataCryptData0;
    //    strSendDataCryptData.append("\0");

        QString  AIINPUTSERVER =  "http://hw.ai-input.com:19001/klaiwebapi?";

        QString strURL=AIINPUTSERVER;
        strURL=strURL+"KLYCloudReco__"+strSendDataValue;

        qDebug()<<"App strURL:"<<strURL;

        QJsonObject strGetDataByGetMethodFromServer;
        int iRec = GetDataByGetMethodFromServer(strURL,2000,strGetDataByGetMethodFromServer);

        qDebug()<<"strGetDataByGetMethodFromServer:"<<strGetDataByGetMethodFromServer;

        if (iRec==200)
        {
  //          QJsonDocument jsonDocument = QJsonDocument::fromJson(strGetDataByGetMethodFromServer.toLocal8Bit().data());
//            if(jsonDocument.isNull())
//            {
//            qDebug()<< "String NULL"<< strGetDataByGetMethodFromServer.toLocal8Bit().data();
//            }
            QJsonObject json_object = strGetDataByGetMethodFromServer;


                        qDebug()<<"App result:"<<json_object.value("result").toString();

                         ui->lineEdit_RecResult->setText(json_object.value("result").toString());
        }

#endif

#if 1
        m_bMatchMulti=1;
        m_uMatchRange=480;
//        //  /*pThis->*/m_uDotNumber<5 ? /*pThis->*/CharReco_CharRecoClear() : /*pThis->*/CharRecoSvrRecognize(m_pPoint, m_iPoint, m_pStroke, m_iStroke, m_pCharaterBuffer,  m_bMatchMulti,m_uMatchRange);


        if (CharRecoSvrRecognize)
            CharRecoSvrRecognize(m_pPoint, m_iPoint, m_pStroke, m_iStroke, m_pCharaterBuffer,  m_bMatchMulti,m_uMatchRange);
        else
        {
            qDebug()<<"CharRecoSvrRecognize==null";
            return;
        }



          qDebug("m_pCharaterBuffer[0]:%02x",m_pCharaterBuffer[0]);
#endif
   //     printf("app CharRecoSvr get m_pCharater:%02x ",m_pCharaterBuffer[0]);
//        qDebug("app CharRecoSvr get m_pCharater:%02x ",m_pCharaterBuffer[0]);
//        WCHAR *wUniChar = (WCHAR *)m_pCharaterBuffer;
//    //    qDebug("wUniChar:%ws",wUniChar);
//        char  szChStr[30] = "";
//        char str[100]={0};
//        WideCharToMultiByte(936,0,wUniChar,5,szChStr,30,NULL,NULL);
//        sprintf(str,"%s ",szChStr);
//        OutputDebugStringA(str);






        QChar* unicode=new QChar[5];

        for (int i=0;i<5;i++)
            unicode[i]=m_pCharaterBuffer[i];

        QString str1=QString::fromRawData(unicode,5);//得到字符串结果



        ui->lineEdit_RecResult->setText(str1);//输出结果

        //            sprintf(str,"app CharReco get m_pCharater:%02x ",m_pCharaterBuffer[0]);
        //            OutputDebugStringA(str);

        //            qDebug()<<"m_pCharaterBuffer:"<<m_pCharaterBuffer;


        CharReco_CharRecoClear();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    // 这里必须使用buttons()
    if(event->buttons() & Qt::LeftButton) //进行的按位与
    {
//        qDebug()<<"Move x:"<<event->x()<<"y:"<<event->y();

        endPoint = event->pos();
        update();
        DrawScreen(event->x(),event->y(),0);

    }
    if (isDrawing && (event->buttons() & Qt::LeftButton)) {
        drawPenStroke(lastPoint, event->pos());
        prevPoint = lastPoint;
        lastPoint = event->pos();
    }
}



void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isDrawing = false;
        isLiftingPen = true; // 鼠标释放，标记为正在提笔
        // 再次调用绘制函数，处理提笔时的变细效果
        drawPenStroke(lastPoint, event->pos());
    }
    qDebug()<<"mouseReleaseEvent"<<event->button();
    //    if(event->button() == Qt::LeftButton)
    if(event->button()== Qt::LeftButton) //进行的按位与
    {
        qDebug()<<"Release x:"<<event->x()<<"y:"<<event->y();
        lastPoint = event->pos();

        endPoint = event->pos();
        update();
        DrawScreen(0, 0,0);

    }
}

MainWindow::~MainWindow()
{
    workerThread->quit();
    workerThread->wait();
    delete worker;
    delete workerThread;
    CharReco_CharRecoUninit();
    delete ui;

}
void MainWindow::startTask() {
    //ui->resultLabel->setText("任务进行中...");
    QMetaObject::invokeMethod(worker, "doWork", Qt::QueuedConnection);
}
void MainWindow::handleResult(unsigned short x, unsigned short y, unsigned char b, unsigned char b1) {
    // 当前绘图点
    QPoint pt;
    pt.setX(x * width / 1200);
    pt.setY(y * height / 2048);

    if (x != 0 && y != 0) {
        if (b == 8) {  // 按下
            if (bFirstPress == false) {
                qDebug("b==8 press first pendown");
                bFirstPress = true;
                path.moveTo(pt); // 记录按下的起始位置

                lastPoint = pt;
                prevPoint = lastPoint;
                isDrawing = true;
                isLiftingPen = false; // 开始书写，重置提笔标记
                drawPenStroke(lastPoint, lastPoint); // 绘制起始点
            } else {
                path.lineTo(pt); // 连线
                drawPenStroke(lastPoint, pt);
                prevPoint = lastPoint;
                lastPoint = pt;
            }

            POINT point;
            point.x = pt.x();
            point.y = pt.y();
            CharReco_Stroking(point); // 手写时的数据处理
            update();

            // 停止定时器（提笔后重新启动定时器）
            if (isPenUp) {
                penUpTimer->start();  // 启动定时器
                isPenUp = false;      // 重置提笔标志
            }
        }

        // 提笔时（b == 0），重置计时器
        penUpTimer->stop();  // 停止计时器
        isPenUp = true;
    } else {  // 提笔时 (b == 0)
        if (b == 0 && bFirstPress == true) {
            // 提笔后需要更新状态
            bFirstPress = false;
            update();  // 更新界面
            CharReco_StrokeEnd();  // 结束书写

            isDrawing = false;
            isLiftingPen = true; // 鼠标释放，标记为正在提笔
            drawPenStroke(lastPoint, pt); // 再次绘制最后一笔，处理提笔时的效果

            // 启动定时器
            if (!penUpTimer->isActive()) {
                penUpTimer->start();
            }
        }
    }
}
void MainWindow::clearCanvas() {
    qDebug() << "Clearing canvas due to 3 seconds pen-up timeout";

    // 清空画布，重新填充成透明
    canvas.fill(Qt::transparent);
    update();  // 触发重绘

    // 清除字符识别相关的参数
    CharRecoGetPara(m_pPoint, m_iPoint, m_pStroke, m_iStroke);

    m_bMatchMulti = 1;
    m_uMatchRange = 480;

    if (CharRecoSvrRecognize) {
        CharRecoSvrRecognize(m_pPoint, m_iPoint, m_pStroke, m_iStroke, m_pCharaterBuffer, m_bMatchMulti, m_uMatchRange);
    } else {
        qDebug() << "CharRecoSvrRecognize == null";
        return;
    }

    qDebug("m_pCharaterBuffer[0]: %02x", m_pCharaterBuffer[0]);

    QChar *unicode = new QChar[5];
    for (int i = 0; i < 5; i++) {
        unicode[i] = m_pCharaterBuffer[i];
    }

    QString str1 = QString::fromRawData(unicode, 5); // 得到字符串结果
    ui->lineEdit_RecResult->setText(str1); // 输出识别结果

    delete [] unicode;

    CharReco_CharRecoClear(); // 清空识别数据
}
