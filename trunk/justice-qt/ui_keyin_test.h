/********************************************************************************
** Form generated from reading UI file 'keyin_test.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KEYIN_TEST_H
#define UI_KEYIN_TEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_KeyIn_test
{
public:
    QLabel *label;
    QCheckBox *checkBox;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QTextEdit *TestCount_Edit;
    QTextEdit *OPCode;
    QTextEdit *DW1_Edit;
    QTextEdit *DW10_Edit;
    QTextEdit *DW11_Edit;
    QTextEdit *DW12_Edit;
    QPushButton *Save_pushButton;
    QPushButton *Load_pushButton;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QLabel *label_7;

    void setupUi(QDialog *KeyIn_test)
    {
        if (KeyIn_test->objectName().isEmpty())
            KeyIn_test->setObjectName(QStringLiteral("KeyIn_test"));
        KeyIn_test->resize(491, 366);
        label = new QLabel(KeyIn_test);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(34, 30, 101, 17));
        QFont font;
        font.setPointSize(13);
        label->setFont(font);
        checkBox = new QCheckBox(KeyIn_test);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(48, 70, 91, 22));
        QFont font1;
        font1.setPointSize(12);
        checkBox->setFont(font1);
        label_2 = new QLabel(KeyIn_test);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(54, 110, 81, 17));
        label_2->setFont(font);
        label_3 = new QLabel(KeyIn_test);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(80, 150, 61, 17));
        label_3->setFont(font1);
        label_4 = new QLabel(KeyIn_test);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(80, 190, 70, 17));
        label_4->setFont(font1);
        label_5 = new QLabel(KeyIn_test);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(80, 230, 67, 17));
        label_5->setFont(font1);
        label_6 = new QLabel(KeyIn_test);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(80, 270, 67, 17));
        label_6->setFont(font1);
        pushButton = new QPushButton(KeyIn_test);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(280, 60, 60, 30));
        pushButton->setIconSize(QSize(25, 25));
        pushButton_2 = new QPushButton(KeyIn_test);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(340, 60, 60, 30));
        pushButton_2->setIconSize(QSize(25, 20));
        TestCount_Edit = new QTextEdit(KeyIn_test);
        TestCount_Edit->setObjectName(QStringLiteral("TestCount_Edit"));
        TestCount_Edit->setGeometry(QRect(150, 26, 81, 28));
        OPCode = new QTextEdit(KeyIn_test);
        OPCode->setObjectName(QStringLiteral("OPCode"));
        OPCode->setGeometry(QRect(150, 105, 51, 28));
        DW1_Edit = new QTextEdit(KeyIn_test);
        DW1_Edit->setObjectName(QStringLiteral("DW1_Edit"));
        DW1_Edit->setGeometry(QRect(150, 145, 111, 28));
        DW10_Edit = new QTextEdit(KeyIn_test);
        DW10_Edit->setObjectName(QStringLiteral("DW10_Edit"));
        DW10_Edit->setGeometry(QRect(150, 185, 111, 28));
        DW11_Edit = new QTextEdit(KeyIn_test);
        DW11_Edit->setObjectName(QStringLiteral("DW11_Edit"));
        DW11_Edit->setGeometry(QRect(150, 225, 111, 28));
        DW12_Edit = new QTextEdit(KeyIn_test);
        DW12_Edit->setObjectName(QStringLiteral("DW12_Edit"));
        DW12_Edit->setGeometry(QRect(150, 265, 111, 28));
        Save_pushButton = new QPushButton(KeyIn_test);
        Save_pushButton->setObjectName(QStringLiteral("Save_pushButton"));
        Save_pushButton->setGeometry(QRect(280, 20, 60, 30));
        Load_pushButton = new QPushButton(KeyIn_test);
        Load_pushButton->setObjectName(QStringLiteral("Load_pushButton"));
        Load_pushButton->setGeometry(QRect(340, 20, 60, 30));
        pushButton_3 = new QPushButton(KeyIn_test);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(400, 20, 60, 30));
        pushButton_4 = new QPushButton(KeyIn_test);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(400, 60, 60, 30));
        label_7 = new QLabel(KeyIn_test);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(30, 310, 431, 17));

        retranslateUi(KeyIn_test);

        QMetaObject::connectSlotsByName(KeyIn_test);
    } // setupUi

    void retranslateUi(QDialog *KeyIn_test)
    {
        KeyIn_test->setWindowTitle(QApplication::translate("KeyIn_test", "Dialog", 0));
        label->setText(QApplication::translate("KeyIn_test", "Test Count : ", 0));
        checkBox->setText(QApplication::translate("KeyIn_test", "Admin", 0));
        label_2->setText(QApplication::translate("KeyIn_test", "OP code : ", 0));
        label_3->setText(QApplication::translate("KeyIn_test", "DW 1 : ", 0));
        label_4->setText(QApplication::translate("KeyIn_test", "DW 10 : ", 0));
        label_5->setText(QApplication::translate("KeyIn_test", "DW 11 : ", 0));
        label_6->setText(QApplication::translate("KeyIn_test", "DW 12 :", 0));
        pushButton->setText(QApplication::translate("KeyIn_test", "Run", 0));
        pushButton_2->setText(QApplication::translate("KeyIn_test", "Stop", 0));
        Save_pushButton->setText(QApplication::translate("KeyIn_test", "Save", 0));
        Load_pushButton->setText(QApplication::translate("KeyIn_test", "Load", 0));
        pushButton_3->setText(QApplication::translate("KeyIn_test", "Clear", 0));
        pushButton_4->setText(QApplication::translate("KeyIn_test", "Close", 0));
        label_7->setText(QApplication::translate("KeyIn_test", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class KeyIn_test: public Ui_KeyIn_test {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KEYIN_TEST_H
