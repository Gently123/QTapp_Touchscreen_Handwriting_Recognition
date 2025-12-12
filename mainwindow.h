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

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPixmap>
#include <QPainterPath>
#include <cmath>
#include <ctime>
#include <QVector>
#include <QMenu>
#include <QAction>
#include <QElapsedTimer>
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
    void clearCanvas();
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
    void showMessage();

protected:
//    void paintEvent(QPaintEvent *) override {
//        QPainter painter(this);
//        painter.drawPixmap(0, 0, canvas);
//    }

    void resizeEvent(QResizeEvent *) override {
        QPixmap newCanvas(size());
        newCanvas.fill(Qt::transparent);
        QPainter painter(&newCanvas);
        painter.drawPixmap(0, 0, canvas);
        canvas = newCanvas;
    }

//    void mousePressEvent(QMouseEvent *event) override {
//        if (event->button() == Qt::LeftButton) {
//            lastPoint = event->pos();
//            prevPoint = lastPoint;
//            isDrawing = true;
//            isLiftingPen = false; // 开始书写，重置提笔标记
//            drawPenStroke(lastPoint, lastPoint);
//        }
//    }

//    void mouseMoveEvent(QMouseEvent *event) override {
//        if (isDrawing && (event->buttons() & Qt::LeftButton)) {
//            drawPenStroke(lastPoint, event->pos());
//            prevPoint = lastPoint;
//            lastPoint = event->pos();
//        }
//    }

//    void mouseReleaseEvent(QMouseEvent *event) override {
//        if (event->button() == Qt::LeftButton) {
//            isDrawing = false;
//            isLiftingPen = true; // 鼠标释放，标记为正在提笔
//            // 再次调用绘制函数，处理提笔时的变细效果
//            drawPenStroke(lastPoint, event->pos());
//        }
//    }

    void contextMenuEvent(QContextMenuEvent *event) override {
        QMenu menu(this);
        QAction *clearAction = menu.addAction("清屏");
        connect(clearAction, &QAction::triggered, this, [this]() {
            canvas.fill(Qt::white);
            update();
        });
        menu.exec(event->globalPos());
    }
private:
    Ui::MainWindow *ui;
    QPixmap pix;
    QPoint lastPoint;
    QPoint endPoint;

    QThread *workerThread;
    WorkerThread *worker;
    QPainterPath path;

    QPixmap canvas;
//    QPoint lastPoint;
    QPoint prevPoint;
    bool isDrawing;
    QColor penColor;
    double smoothedSpeed;
    double lastAngle;
    bool isLiftingPen; // 新增变量，用于标记是否正在提笔
    QElapsedTimer penUpTime;  // 用于跟踪提笔后的时间
    QTimer* penUpTimer;  // 用于控制提笔超过3秒时清空画布
     bool isPenUp;        // 用于标记是否提笔

    void drawPenStroke(const QPoint &from, const QPoint &to) {
        QPainter painter(&canvas);
        painter.setRenderHint(QPainter::Antialiasing);

        // 计算速度
        double speed = std::sqrt(std::pow(to.x() - from.x(), 2) + std::pow(to.y() - from.y(), 2));

        // 低通滤波，平滑速度变化，减少突变
        smoothedSpeed = smoothedSpeed * 0.8 + speed * 0.2;

        // 计算笔触的角度和宽度（模拟毛笔笔尖的形状）
        double currentAngle = std::atan2(to.y() - from.y(), to.x() - from.x());
        double angleDiff = std::abs(currentAngle - lastAngle);

        // 模拟压力感应，速度越慢压力越大，笔触越粗
        int baseWidth = 20;
        int minWidth = 2;
        int maxWidth = 30;
        int penWidth = qBound(minWidth, static_cast<int>(baseWidth - smoothedSpeed / 3), maxWidth);

        // 起笔和收笔效果，让笔锋更锋利
        if (isDrawing && (from == to)) {
            penWidth = maxWidth; // 起笔加粗
        }

        // 提笔时变细
        if (isLiftingPen) {
            penWidth = 1;
        }

        // 转折处让笔锋更锋利
        if (angleDiff > 0.5) {
            penWidth = minWidth;
        }

        // 设定透明度以模拟墨水效果
        int alpha = 200 - static_cast<int>(smoothedSpeed);
        if (alpha>0)
        penColor.setAlpha(alpha);

        QPen pen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(pen);

        // 使用贝塞尔曲线来平滑笔迹
        QPainterPath path;
        if (isDrawing && prevPoint != from) {
            QPointF controlPoint = QPointF((prevPoint.x() + from.x()) / 2, (prevPoint.y() + from.y()) / 2);
            path.moveTo(prevPoint);
            path.quadTo(controlPoint, from);
        } else {
            path.moveTo(from);
        }

        // 当角度变化较大时，调整笔锋效果
        if (angleDiff > 0.5) {
            // 根据速度和角度动态调整控制点位置
            double controlOffset = penWidth * (1.0 + smoothedSpeed / 10.0);
            QPointF controlPoint1 = from + QPointF(std::cos(lastAngle + M_PI / 2) * controlOffset, std::sin(lastAngle + M_PI / 2) * controlOffset);
            QPointF controlPoint2 = to + QPointF(std::cos(currentAngle + M_PI / 2) * controlOffset, std::sin(currentAngle + M_PI / 2) * controlOffset);
            path.cubicTo(controlPoint1, controlPoint2, to);
        } else {
            if (from != to) {
                QPointF controlPoint = QPointF((from.x() + to.x()) / 2, (from.y() + to.y()) / 2);
                path.quadTo(controlPoint, to);
            }
        }

        // 绘制路径
        painter.setPen(pen);
        painter.drawPath(path);

        lastAngle = currentAngle;
        update();
    }


};

#endif // MAINWINDOW_H
