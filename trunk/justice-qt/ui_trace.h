/********************************************************************************
** Form generated from reading UI file 'trace.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRACE_H
#define UI_TRACE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Trace
{
public:
    QPushButton *save_button;
    QPushButton *close_button;

    void setupUi(QDialog *Trace)
    {
        if (Trace->objectName().isEmpty())
            Trace->setObjectName(QStringLiteral("Trace"));
        Trace->resize(957, 684);
        save_button = new QPushButton(Trace);
        save_button->setObjectName(QStringLiteral("save_button"));
        save_button->setGeometry(QRect(850, 597, 85, 30));
        close_button = new QPushButton(Trace);
        close_button->setObjectName(QStringLiteral("close_button"));
        close_button->setGeometry(QRect(850, 630, 85, 30));

        retranslateUi(Trace);

        QMetaObject::connectSlotsByName(Trace);
    } // setupUi

    void retranslateUi(QDialog *Trace)
    {
        Trace->setWindowTitle(QApplication::translate("Trace", "Dialog", 0));
        save_button->setText(QApplication::translate("Trace", "Save", 0));
        close_button->setText(QApplication::translate("Trace", "Close", 0));
    } // retranslateUi

};

namespace Ui {
    class Trace: public Ui_Trace {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRACE_H
