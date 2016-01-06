/********************************************************************************
** Form generated from reading UI file 'sel_dev.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEL_DEV_H
#define UI_SEL_DEV_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_sel_dev
{
public:
    QTableView *tableView;
    QPushButton *OK;
    QPushButton *Close;
    QRadioButton *Radio_port0_SSD;
    QRadioButton *Radio_port1_SSD;

    void setupUi(QDialog *sel_dev)
    {
        if (sel_dev->objectName().isEmpty())
            sel_dev->setObjectName(QStringLiteral("sel_dev"));
        sel_dev->resize(555, 136);
        QFont font;
        font.setPointSize(10);
        sel_dev->setFont(font);
        tableView = new QTableView(sel_dev);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setGeometry(QRect(20, 10, 451, 108));
        tableView->setFont(font);
        OK = new QPushButton(sel_dev);
        OK->setObjectName(QStringLiteral("OK"));
        OK->setGeometry(QRect(480, 10, 65, 24));
        QFont font1;
        font1.setPointSize(11);
        OK->setFont(font1);
        Close = new QPushButton(sel_dev);
        Close->setObjectName(QStringLiteral("Close"));
        Close->setGeometry(QRect(480, 40, 65, 24));
        Close->setFont(font1);
        Radio_port0_SSD = new QRadioButton(sel_dev);
        Radio_port0_SSD->setObjectName(QStringLiteral("Radio_port0_SSD"));
        Radio_port0_SSD->setGeometry(QRect(40, 48, 301, 22));
        Radio_port1_SSD = new QRadioButton(sel_dev);
        Radio_port1_SSD->setObjectName(QStringLiteral("Radio_port1_SSD"));
        Radio_port1_SSD->setGeometry(QRect(40, 78, 281, 22));

        retranslateUi(sel_dev);

        QMetaObject::connectSlotsByName(sel_dev);
    } // setupUi

    void retranslateUi(QDialog *sel_dev)
    {
        sel_dev->setWindowTitle(QApplication::translate("sel_dev", "Dialog", 0));
        OK->setText(QApplication::translate("sel_dev", "OK", 0));
        Close->setText(QApplication::translate("sel_dev", "Close", 0));
        Radio_port0_SSD->setText(QApplication::translate("sel_dev", "Port 0 SSD                              dev/nvme0n1  ", 0));
        Radio_port1_SSD->setText(QApplication::translate("sel_dev", "port 1 SSD                              dev/nvme1n1  ", 0));
    } // retranslateUi

};

namespace Ui {
    class sel_dev: public Ui_sel_dev {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEL_DEV_H
