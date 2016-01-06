/********************************************************************************
** Form generated from reading UI file 'pcie.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PCIE_H
#define UI_PCIE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_PCIE
{
public:
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLabel *label_6;
    QPushButton *Stop1;
    QPushButton *pushButton_21;
    QPushButton *pushButton_22;
    QPushButton *pushButton_28;
    QLabel *CDM_input_label;
    QPushButton *Run1;
    QLabel *label_13;
    QLabel *name_label;
    QPushButton *Clear;
    QPushButton *Idfy0_Button;
    QPushButton *Idfy1_Button;
    QPushButton *PCI_Button;
    QPushButton *CTRL_Button;
    QPushButton *Histoty_button;
    QPushButton *Command_button;

    void setupUi(QDialog *PCIE)
    {
        if (PCIE->objectName().isEmpty())
            PCIE->setObjectName(QStringLiteral("PCIE"));
        PCIE->resize(1004, 770);
        QFont font;
        font.setPointSize(10);
        font.setBold(false);
        font.setWeight(50);
        PCIE->setFont(font);
        PCIE->setLayoutDirection(Qt::LeftToRight);
        lineEdit = new QLineEdit(PCIE);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(770, 728, 201, 27));
        lineEdit_2 = new QLineEdit(PCIE);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(96, 505, 101, 27));
        label_6 = new QLabel(PCIE);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(20, 510, 75, 16));
        Stop1 = new QPushButton(PCIE);
        Stop1->setObjectName(QStringLiteral("Stop1"));
        Stop1->setGeometry(QRect(62, 30, 50, 25));
        QFont font1;
        font1.setPointSize(10);
        Stop1->setFont(font1);
        pushButton_21 = new QPushButton(PCIE);
        pushButton_21->setObjectName(QStringLiteral("pushButton_21"));
        pushButton_21->setGeometry(QRect(165, 30, 50, 25));
        pushButton_21->setFont(font1);
        pushButton_22 = new QPushButton(PCIE);
        pushButton_22->setObjectName(QStringLiteral("pushButton_22"));
        pushButton_22->setGeometry(QRect(216, 30, 50, 25));
        pushButton_22->setFont(font1);
        pushButton_28 = new QPushButton(PCIE);
        pushButton_28->setObjectName(QStringLiteral("pushButton_28"));
        pushButton_28->setGeometry(QRect(114, 30, 50, 25));
        pushButton_28->setFont(font1);
        CDM_input_label = new QLabel(PCIE);
        CDM_input_label->setObjectName(QStringLiteral("CDM_input_label"));
        CDM_input_label->setGeometry(QRect(695, 733, 71, 20));
        Run1 = new QPushButton(PCIE);
        Run1->setObjectName(QStringLiteral("Run1"));
        Run1->setGeometry(QRect(12, 30, 50, 25));
        label_13 = new QLabel(PCIE);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(16, 720, 311, 26));
        label_13->setCursor(QCursor(Qt::IBeamCursor));
        name_label = new QLabel(PCIE);
        name_label->setObjectName(QStringLiteral("name_label"));
        name_label->setGeometry(QRect(350, 724, 281, 17));
        Clear = new QPushButton(PCIE);
        Clear->setObjectName(QStringLiteral("Clear"));
        Clear->setGeometry(QRect(266, 30, 50, 25));
        Idfy0_Button = new QPushButton(PCIE);
        Idfy0_Button->setObjectName(QStringLiteral("Idfy0_Button"));
        Idfy0_Button->setGeometry(QRect(317, 30, 50, 25));
        Idfy1_Button = new QPushButton(PCIE);
        Idfy1_Button->setObjectName(QStringLiteral("Idfy1_Button"));
        Idfy1_Button->setGeometry(QRect(367, 30, 50, 25));
        PCI_Button = new QPushButton(PCIE);
        PCI_Button->setObjectName(QStringLiteral("PCI_Button"));
        PCI_Button->setGeometry(QRect(417, 30, 55, 25));
        CTRL_Button = new QPushButton(PCIE);
        CTRL_Button->setObjectName(QStringLiteral("CTRL_Button"));
        CTRL_Button->setGeometry(QRect(472, 30, 55, 25));
        Histoty_button = new QPushButton(PCIE);
        Histoty_button->setObjectName(QStringLiteral("Histoty_button"));
        Histoty_button->setGeometry(QRect(527, 30, 50, 25));
        Command_button = new QPushButton(PCIE);
        Command_button->setObjectName(QStringLiteral("Command_button"));
        Command_button->setGeometry(QRect(578, 30, 50, 25));
        pushButton_21->raise();
        Run1->raise();
        lineEdit->raise();
        lineEdit_2->raise();
        label_6->raise();
        Stop1->raise();
        pushButton_22->raise();
        pushButton_28->raise();
        CDM_input_label->raise();
        label_13->raise();
        name_label->raise();
        Clear->raise();
        Idfy0_Button->raise();
        Idfy1_Button->raise();
        PCI_Button->raise();
        CTRL_Button->raise();
        Histoty_button->raise();
        Command_button->raise();

        retranslateUi(PCIE);

        QMetaObject::connectSlotsByName(PCIE);
    } // setupUi

    void retranslateUi(QDialog *PCIE)
    {
        PCIE->setWindowTitle(QApplication::translate("PCIE", "Dialog", 0));
        label_6->setText(QApplication::translate("PCIE", "Script path : ", 0));
        Stop1->setText(QApplication::translate("PCIE", "Stop", 0));
        pushButton_21->setText(QApplication::translate("PCIE", "WBUF", 0));
        pushButton_22->setText(QApplication::translate("PCIE", "RBUF", 0));
        pushButton_28->setText(QApplication::translate("PCIE", "View", 0));
        CDM_input_label->setText(QApplication::translate("PCIE", "CMD input : ", 0));
        Run1->setText(QApplication::translate("PCIE", "Run", 0));
        label_13->setText(QApplication::translate("PCIE", "Elapsed : ", 0));
        name_label->setText(QApplication::translate("PCIE", "Script name : ", 0));
        Clear->setText(QApplication::translate("PCIE", "Clear", 0));
        Idfy0_Button->setText(QApplication::translate("PCIE", "IDFY0", 0));
        Idfy1_Button->setText(QApplication::translate("PCIE", "IDFY1", 0));
        PCI_Button->setText(QApplication::translate("PCIE", "PCIReg", 0));
        CTRL_Button->setText(QApplication::translate("PCIE", "CtrlReg", 0));
        Histoty_button->setText(QApplication::translate("PCIE", "Trace", 0));
        Command_button->setText(QApplication::translate("PCIE", "CMD", 0));
    } // retranslateUi

};

namespace Ui {
    class PCIE: public Ui_PCIE {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PCIE_H
