#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowFlags(w.windowFlags() | Qt::WindowStaysOnTopHint);
    w.showFullScreen();  // 直接全屏显示


    return a.exec();
}
