#include "mainwindow.h"
#include <QApplication>
#include <sel_dev.h>
#include <stdio.h>
#include <QMessageBox>
#include <QThread>
#include <log_setting.h>
#include <pcie.h>
#include <QDesktopWidget>

QString serach_jdriver_str;
QString filePath_args;
int m_argc;

int main(int argc, char *argv[])
{

   QApplication a(argc, argv);

   QStringList args = a.arguments();

    m_argc = argc;

    char COM_str[ 100 ];

     for( int i = 1 ; i < args.size() ; i++ )
       {

         if( i == 1 )
           {
              sprintf( COM_str , "%s" , argv[i] );
              filePath_args  = COM_str ;
           }

       }

     QProcess proc1;      // Start up server
     proc1.start("xterm redis-server");

     QProcess proc2;
     proc2.start("sh", QStringList() << "-c" << "lsmod | grep jdoor" );
     proc2.waitForFinished();

     serach_jdriver_str = proc2.readAll();

     //---- Jdriver exist ? Need to Install Jdriver ? ----------
     if( strlen( serach_jdriver_str.toLatin1() ) <= 0 )
       {

           QMessageBox::information( NULL,    "Jdriver"  ,   "  No installed driver , please install jdoor.ko and nvme.ko first                    "  , QMessageBox::Ok );
           return true; //a.exec();

       }

     PCIE  P;
     P.show();

     sel_dev  S_dev;    // select driver GUI
     S_dev.move(QApplication::desktop()->screen()->rect().center() - S_dev.rect().center());

     return a.exec();

}
























