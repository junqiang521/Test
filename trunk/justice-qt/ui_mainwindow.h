/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *start1;
    QPushButton *stop1;
    QPushButton *start2;
    QPushButton *stop2;
    QListWidget *listinfo1;
    QListWidget *listinfo2;
    QPushButton *CLose;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(668, 570);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        start1 = new QPushButton(centralWidget);
        start1->setObjectName(QStringLiteral("start1"));
        start1->setGeometry(QRect(30, 470, 71, 27));
        QFont font;
        font.setPointSize(10);
        start1->setFont(font);
        stop1 = new QPushButton(centralWidget);
        stop1->setObjectName(QStringLiteral("stop1"));
        stop1->setGeometry(QRect(110, 470, 71, 27));
        stop1->setFont(font);
        start2 = new QPushButton(centralWidget);
        start2->setObjectName(QStringLiteral("start2"));
        start2->setGeometry(QRect(470, 470, 71, 27));
        start2->setFont(font);
        stop2 = new QPushButton(centralWidget);
        stop2->setObjectName(QStringLiteral("stop2"));
        stop2->setGeometry(QRect(550, 470, 71, 27));
        stop2->setFont(font);
        listinfo1 = new QListWidget(centralWidget);
        listinfo1->setObjectName(QStringLiteral("listinfo1"));
        listinfo1->setGeometry(QRect(30, 20, 291, 441));
        listinfo2 = new QListWidget(centralWidget);
        listinfo2->setObjectName(QStringLiteral("listinfo2"));
        listinfo2->setGeometry(QRect(330, 20, 291, 441));
        CLose = new QPushButton(centralWidget);
        CLose->setObjectName(QStringLiteral("CLose"));
        CLose->setGeometry(QRect(190, 470, 71, 27));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 668, 25));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        start1->setText(QApplication::translate("MainWindow", "Start", 0));
        stop1->setText(QApplication::translate("MainWindow", "Stop", 0));
        start2->setText(QApplication::translate("MainWindow", "Start", 0));
        stop2->setText(QApplication::translate("MainWindow", "Stop", 0));
        CLose->setText(QApplication::translate("MainWindow", "Close", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
