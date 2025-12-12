/********************************************************************************
** Form generated from reading UI file 'threads.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_THREADS_H
#define UI_THREADS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_threads
{
public:
    QLabel *resultLabel;

    void setupUi(QWidget *threads)
    {
        if (threads->objectName().isEmpty())
            threads->setObjectName(QStringLiteral("threads"));
        threads->resize(1139, 710);
        resultLabel = new QLabel(threads);
        resultLabel->setObjectName(QStringLiteral("resultLabel"));
        resultLabel->setGeometry(QRect(101, 80, 241, 20));

        retranslateUi(threads);

        QMetaObject::connectSlotsByName(threads);
    } // setupUi

    void retranslateUi(QWidget *threads)
    {
        threads->setWindowTitle(QApplication::translate("threads", "Form", Q_NULLPTR));
        resultLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class threads: public Ui_threads {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_THREADS_H
