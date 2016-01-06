/********************************************************************************
** Form generated from reading UI file 'buffer_setting.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BUFFER_SETTING_H
#define UI_BUFFER_SETTING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_buffer_setting
{
public:
    QDialogButtonBox *buttonBox;
    QLineEdit *buf_len_lineEdit;
    QLabel *Buffer_len_label;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEdit;
    QLabel *label_3;
    QPushButton *pushButton;

    void setupUi(QDialog *buffer_setting)
    {
        if (buffer_setting->objectName().isEmpty())
            buffer_setting->setObjectName(QStringLiteral("buffer_setting"));
        buffer_setting->resize(582, 329);
        buttonBox = new QDialogButtonBox(buffer_setting);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(480, 40, 81, 61));
        buttonBox->setOrientation(Qt::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buf_len_lineEdit = new QLineEdit(buffer_setting);
        buf_len_lineEdit->setObjectName(QStringLiteral("buf_len_lineEdit"));
        buf_len_lineEdit->setGeometry(QRect(140, 36, 111, 25));
        Buffer_len_label = new QLabel(buffer_setting);
        Buffer_len_label->setObjectName(QStringLiteral("Buffer_len_label"));
        Buffer_len_label->setGeometry(QRect(30, 40, 101, 17));
        label = new QLabel(buffer_setting);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(260, 40, 51, 17));
        label_2 = new QLabel(buffer_setting);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 80, 91, 17));
        lineEdit = new QLineEdit(buffer_setting);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(140, 80, 113, 27));
        label_3 = new QLabel(buffer_setting);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(260, 83, 81, 17));
        pushButton = new QPushButton(buffer_setting);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(346, 80, 21, 27));

        retranslateUi(buffer_setting);
        QObject::connect(buttonBox, SIGNAL(accepted()), buffer_setting, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), buffer_setting, SLOT(reject()));

        QMetaObject::connectSlotsByName(buffer_setting);
    } // setupUi

    void retranslateUi(QDialog *buffer_setting)
    {
        buffer_setting->setWindowTitle(QApplication::translate("buffer_setting", "Dialog", 0));
        Buffer_len_label->setText(QApplication::translate("buffer_setting", "Buffer length :", 0));
        label->setText(QApplication::translate("buffer_setting", "( byte )", 0));
        label_2->setText(QApplication::translate("buffer_setting", "Ini setting : ", 0));
        label_3->setText(QApplication::translate("buffer_setting", "( test case )", 0));
        pushButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class buffer_setting: public Ui_buffer_setting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BUFFER_SETTING_H
