#include "log_setting.h"
#include "ui_log_setting.h"
#include <pcie.h>
#include  <QMessageBox>

bool m_save_log_checked;
bool m_modify_log_self_checked;
QString log_name_self;

Log_setting::Log_setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Log_setting)
{

    ui->setupUi(this);
    this->setWindowTitle( "Log setting" );

    if( ui->Save_log->isChecked() )
      check_save_log = true;
    else
      check_save_log = false;

    if( ui->Log_name->isChecked() )
      check_change_loge_name = true;
    else
      check_change_loge_name = false;

}

Log_setting::~Log_setting()
{
    delete ui;
}


void Log_setting::on_buttonBox_rejected()
{


}

void Log_setting::on_buttonBox_accepted()
{


    if( ui->Save_log->isChecked() )
      check_save_log = true;
    else
      check_save_log = false;

    if( ui->Log_name->isChecked() )
      check_change_loge_name = true;
    else
      check_change_loge_name = false;


    m_save_log_checked = check_save_log;


    if( ui->Log_name->isChecked() )
       {
         m_modify_log_self_checked = true;
         log_name_self = ui->log_name_line->text().toStdString().c_str();

       }
    else
        m_modify_log_self_checked = false;


}

void Log_setting::on_log_name_line_returnPressed()
{


}



void Log_setting::on_log_name_line_cursorPositionChanged(int arg1, int arg2)
{

    char source[ 400 ];
    sprintf( source , "%s" ,    ui->log_name_line->text().toStdString().c_str()  );

    if( strlen( source ) > 0 )
      ui->Log_name->isChecked();

}
















