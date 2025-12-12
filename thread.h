#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QObject>
#include <QThread>
#include <QString>

class WorkerThread : public QObject {
    Q_OBJECT
public:
    WorkerThread() {}
    int lastX;
    int lastY;

public slots:
    void doWork();  // 使用 Q_SLOT 宏标记为槽函数

signals:
    void resultReady(unsigned short x,unsigned short y, unsigned char b,unsigned char b1);
};

#endif // WORKERTHREAD_H
