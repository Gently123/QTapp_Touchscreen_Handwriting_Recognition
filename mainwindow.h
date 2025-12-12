#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>

//#include "windows.h"

#include <QJsonObject>

//#include "CharReco.h"
//#include "CharRecoSvr.h"


#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QTimer>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>

#include <QMainWindow>
#include <QThread>
#include <QString>
#include "thread.h"
#include <QPainter>
#include <QMessageBox>
#include <QPainterPath>
#define  MAX_POINT_NUM          1000  // 一次最多能识别的点数（点数到达 1024 之后，调用 HSCharRecognize 时，识别核心崩溃）
#define  MAX_STROKE_NUM         500   // 最多的笔画数（包含结束标志的笔画至少占两点，故只需最多点数的一半即可）

#define  GAP_PERCENT          30    // 文字拆分最小间隙百分比（相对于文字大小）

#define  MAX_CANDIDATE_NUM    10

typedef struct tagRECT {
  long left;
  long top;
  long right;
  long bottom;
} RECT;

typedef struct tagPOINT {
  long x;
  long y;
} POINT;

struct stStroke
{
    int   iBegin;    // 笔画开始点位置
    int   iEnd;      // 笔画结束点位置
    RECT  rect;      // 笔画外接矩形
};


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int width;
    int height;
    QPoint lastPt;
    bool bFirstPress;
    //CCharReco *m_CharReco;
  //  CCharRecoSvr m_CharRecoSvr;

     bool m_bWritting;			// 写的状态（包括 学习、签名）
    bool  m_bBeginStroke;		// 笔划起始
    unsigned int m_uDotNumber;			// 点阵总数

    bool bIsOnline;
    QNetworkAccessManager *m_pNetworkManager;
    QNetworkReply *m_pNetworkReply;
    int GetDataByGetMethodFromServer(const QString &strUrl,int nHTTP_TIME,QJsonObject &strRetResult);

    short     m_pPoint[MAX_POINT_NUM * 2 + 2];  // 坐标点缓冲区（为汉字结束标志预留一个点）
    int m_iPoint;                   // 坐标点数
    stStroke  m_pStroke[MAX_STROKE_NUM];        // 笔画索引指针
    int m_iStroke;                  // 笔画数
   unsigned short  m_pCharaterBuffer[MAX_CANDIDATE_NUM + 1];
    bool m_bMatchMulti;
    unsigned int  m_uMatchRange;

    void DrawScreen(int nX, int nY, int nZ);

    void OnPenDown(QPoint pt);
    void OnPenMove(QPoint pt, unsigned int  nZ);
    void OnPenUp(QPoint pt);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private slots:
    void handleResult(unsigned short x,unsigned short y, unsigned char b,unsigned char b1);
    void startTask();
    void showMessage() {
            // 当点击右键菜单中的 "Show Message" 时，显示一条信息框
            QMessageBox::information(this, "Tray Icon", "This is a message from the tray icon.");
    }
//protected:
//    void paintEvent(QPaintEvent *event) override {
//            QPainter painter(this);
//            QPen pen;
//            pen.setColor(Qt::blue);   // 设置线条颜色
//            pen.setWidth(3);          // 设置线条宽度
//            painter.setPen(pen);
//            // 绘制曲线路径

//            if (!path.isEmpty()) {
//                painter.drawPath(path);  // 绘制曲线
//            }
//        }
private:
    Ui::MainWindow *ui;
    QPixmap pix;
    QPoint lastPoint;
    QPoint endPoint;

    QThread *workerThread;
    WorkerThread *worker;
    QPainterPath path;



};

#endif // MAINWINDOW_H
