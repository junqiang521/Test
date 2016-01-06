#include "sel_dev.h"
#include "ui_sel_dev.h"
#include "mainwindow.h"
#include <stdio.h>
#include <QLabel>
#include <QDialog>
#include <QFileDialog>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QPoint>
#include <QMessageBox>
#include <QtTest/QTest>
#include <QApplication>
#include <unistd.h>
#include <QListWidget>
#include <QDesktopWidget>
#include <QSettings>
#include <stdio.h>
#include <string.h>
#include <QStatusBar>

extern QString  Product_tilte;
extern int Driver_mode;
extern QString  filePath , file_name;
extern QString fileName_node;
extern QListWidget *Listinfo;
extern char ArgV_P1[ 100 ], ArgV_P2[ 100 ], ArgV_P3[ 100 ];   // ARGV
extern QStringList script_argv_str;
extern QString  Set_DUT_PCIE_number;

//------------------------------------------------------------------------------
extern QLabel *stat_model ;
extern QLabel *stat_capacity ;
extern QLabel *stat_FW_var ;
extern QLabel *stat_MAX_LBA;
extern QLabel *stat_driver_ver;
extern QLabel *stat_pcie_num;
extern QLabel *stat_status_1 ;
//--------------------------------------------------------------------------------

extern char FW_ver[ 200 ];
extern char DUT_name[ 50 ];
extern long DUT_capacity;
extern int  pcie_status;
extern long MAX_LBA;
extern char Jdriver_ver[ 200 ];

char MES[ 100 ];

QStandardItemModel *model_dev = new QStandardItemModel;

sel_dev::sel_dev(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sel_dev)
{

   ui->setupUi(this);
   this->setWindowTitle( "Select NVME SSD" );

   model_dev->setColumnCount( 2 );
   model_dev->setHeaderData( 0,Qt::Horizontal,QString::fromLocal8Bit("  PCIE Port  "));
   model_dev->setHeaderData( 1,Qt::Horizontal,QString::fromLocal8Bit("    NVME SSD device file name    "));

   ui->tableView->setModel( model_dev );
   ui->tableView->setRowHeight(  0 , 8 );
   ui->tableView->setColumnWidth( 0 ,107 );
   ui->tableView->setColumnWidth( 1 , 340 );

   ui->Radio_port0_SSD->setChecked( true );

     QFile DUT_1( "/home/dev/nvme01e1" );

    if( !DUT_1.exists() )
     {
        ui->Radio_port1_SSD->setEnabled( false );
     }

}


void sel_dev::on_tableView_clicked(const QModelIndex &index)
{

}

sel_dev::~sel_dev()
{
    delete ui;
}


void sel_dev::on_OK_clicked()
{

  if( ui->Radio_port0_SSD->isChecked() )
    {
      Set_DUT_PCIE_number = '0';

      fileName_node = "ID.js";
       ArgV_P1[ 0 ] = '0';
       ArgV_P1[ 1 ] ='\0';
    }

  if( ui->Radio_port1_SSD->isChecked() )
    {

      Set_DUT_PCIE_number = '1';

      fileName_node = "ID.js";
       ArgV_P1[ 0 ] = '1';
       ArgV_P1[ 1 ] ='\0';
    }

   WriteSettings();

   sprintf( MES , "  DUT : %d"  ,Set_DUT_PCIE_number.toInt()  );
   stat_pcie_num->setText( MES );


   script_argv_str.clear();
   script_argv_str <<  ArgV_P1;

   //fileName_node = "/home/justice/justice-script/testcase/selectdevice.js";
    fileName_node = "/home/justice/justice-script/command/selectdevice.js";

   M.cmd1->execute(QString( fileName_node  ) , script_argv_str );


   this->close();

}


void sel_dev:: WriteSettings()
{

   QSettings * settings = 0;

   if( QFile( "Justice.ini" ).exists() )
     {

         settings = new QSettings( "Justice.ini", QSettings::IniFormat );
         settings->beginGroup("Setting");
         settings->setValue(  "Set_DUT_PCIE_number" , Set_DUT_PCIE_number.toInt() );

     }
  else
     QMessageBox::information(this,   "Justice"  ,   "                 ini file not exist                          "   , QMessageBox::Ok);

}


void sel_dev::on_pushButton_4_clicked()
{

    this->close();

}

void sel_dev::on_OK_clicked(bool checked)
{

}

void sel_dev::on_OK_released()
{

}

void sel_dev::on_Close_clicked()
{

    this->close();

}
