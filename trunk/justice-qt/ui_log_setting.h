/********************************************************************************
** Form generated from reading UI file 'log_setting.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOG_SETTING_H
#define UI_LOG_SETTING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_Log_setting
{
public:
    QDialogButtonBox *buttonBox;
    QCheckBox *Save_log;
    QCheckBox *Log_name;
    QLineEdit *log_name_line;

    void setupUi(QDialog *Log_setting)
    {
        if (Log_setting->objectName().isEmpty())
            Log_setting->setObjectName(QStringLiteral("Log_setting"));
        Log_setting->resize(762, 490);
        buttonBox = new QDialogButtonBox(Log_setting);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(560, 420, 171, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        Save_log = new QCheckBox(Log_setting);
        Save_log->setObjectName(QStringLiteral("Save_log"));
        Save_log->setGeometry(QRect(20, 20, 101, 22));
        Save_log->setChecked(true);
        Log_name = new QCheckBox(Log_setting);
        Log_name->setObjectName(QStringLiteral("Log_name"));
        Log_name->setGeometry(QRect(20, 50, 151, 22));
        log_name_line = new QLineEdit(Log_setting);
        log_name_line->setObjectName(QStringLiteral("log_name_line"));
        log_name_line->setGeometry(QRect(180, 50, 141, 27));

        retranslateUi(Log_setting);
        QObject::connect(buttonBox, SIGNAL(accepted()), Log_setting, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Log_setting, SLOT(reject()));

        QMetaObject::connectSlotsByName(Log_setting);
    } // setupUi

    void retranslateUi(QDialog *Log_setting)
    {
        Log_setting->setWindowTitle(QApplication::translate("Log_setting", "Dialog", 0));
        Save_log->setText(QApplication::translate("Log_setting", "Save log", 0));
        Log_name->setText(QApplication::translate("Log_setting", "Change log name -", 0));
    } // retranslateUi

};

namespace Ui {
    class Log_setting: public Ui_Log_setting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOG_SETTING_H
