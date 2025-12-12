/********************************************************************************
** Form generated from reading UI file 'thread.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_THREAD_H
#define UI_THREAD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_thread
{
public:

    void setupUi(QWidget *thread)
    {
        if (thread->objectName().isEmpty())
            thread->setObjectName(QStringLiteral("thread"));
        thread->resize(400, 300);

        retranslateUi(thread);

        QMetaObject::connectSlotsByName(thread);
    } // setupUi

    void retranslateUi(QWidget *thread)
    {
        thread->setWindowTitle(QApplication::translate("thread", "Form", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class thread: public Ui_thread {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_THREAD_H
