#include "buffer_setting.h"
#include "ui_buffer_setting.h"
#include <stdio.h>
#include <QMessageBox>
#include <QSettings>
#include <QFile>

char MSE[ 100 ];
extern int buffer_length;
extern QString  Set_default_buffer_length;

buffer_setting::buffer_setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::buffer_setting)
{
    ui->setupUi(this);
    this->setWindowTitle( "Buffer setting" );
    loadSettings();
    buffer_length = ( Set_default_buffer_length.toInt() / 512 );
    sprintf( MSE , "%d" , ( buffer_length * 512 ) );
    ui->buf_len_lineEdit->setText( MSE );
    ui->buf_len_lineEdit->setAlignment(Qt::AlignHCenter);

}


void buffer_setting:: loadSettings()
{

   QSettings * settings = 0;

   if( QFile( "Justice.ini" ).exists() )
    {
         settings = new QSettings( "Justice.ini", QSettings::IniFormat );
         settings->beginGroup("Setting");
         Set_default_buffer_length = settings->value(  "Set_default_buffer_length" ,  1000  ).toString();
         settings->endGroup();

    }
  else
     QMessageBox::information(this,   "Justice"  ,   "                 ini file not exist                          "   , QMessageBox::Ok);

}

void buffer_setting:: WriteSettings()
{

    QSettings * settings = 0;

    if( QFile( "Justice.ini" ).exists() )
      {

          settings = new QSettings( "C0F1GN.ini", QSettings::IniFormat );
          settings->beginGroup("testcase");
          settings->setValue(  "number" ,        ui->lineEdit->text()    );

      }
   else
      QMessageBox::information(this,   "Justice"  ,   "                 ini file not exist                          "   , QMessageBox::Ok);


}









buffer_setting::~buffer_setting()
{

    delete ui;

}


void buffer_setting::on_buttonBox_accepted()
{

    buffer_len = atoi( ui->buf_len_lineEdit->text().toStdString().c_str() );
    buffer_length = ( buffer_len / 512 );

}

void buffer_setting::on_pushButton_clicked()
{
    WriteSettings();

}

void buffer_setting::on_lineEdit_cursorPositionChanged(int arg1, int arg2)
{

}
