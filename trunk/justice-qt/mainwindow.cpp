#include <pcie.h>
#include "justice_cmd.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include <QApplication>
#include <QCloseEvent>
#include <assert.h>
#include <QMessageBox>
#include <QScrollBar>
#include <QTextEdit>
#include <keyin_test.h>

#define SHARED_MEM_SIZE (64 * 1024 * 1024)
unsigned char buf_W[ SHARED_MEM_SIZE ] , buf_R[ SHARED_MEM_SIZE ];

extern QListWidget  *Listinfo;
extern QString Script_file_name;
extern QString filePath;
extern QTextEdit  *TextEdit_display_code;
extern QLineEdit  *lineEdit_Port;
extern QLineEdit  *lineEdit_Delay;
extern int mode_sw;
extern void LOG( QString str );
extern void Save_log( char *str );
extern void Parse_Msg( QString str );
extern int m_argc;
extern QString filePath_args;
extern bool m_save_log_checked;
extern bool m_control_scroll;
extern QListWidget  *Listinfo_W_pattern;
extern QListWidget  *Listinfo_R_pattern;
extern bool command_input_mode;

long line_count;
int buffer_length;



justice_cmd  JUST;

QTimer *timer ;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui( new Ui::MainWindow )

{

    ui->setupUi(this);

    ui->stop1->setEnabled(false);
    ui->stop2->setEnabled(false);

    cmd1 = new justice_cmd;
    cmd2 = new justice_cmd;
    assert(cmd1);
    assert(cmd2);

    connect(cmd1, SIGNAL(started()), this, SLOT(cmd1_started()), Qt::QueuedConnection);
    connect(cmd1, SIGNAL(stopped()), this, SLOT(cmd1_stopped()), Qt::QueuedConnection);
    connect(cmd2, SIGNAL(started()), this, SLOT(cmd2_started()), Qt::QueuedConnection);
    connect(cmd2, SIGNAL(stopped()), this, SLOT(cmd2_stopped()), Qt::QueuedConnection);

    connect(cmd1, SIGNAL(info(QString)), this, SLOT(cmd1_info(QString)), Qt::QueuedConnection);
    connect(cmd1, SIGNAL(err(QString)), this, SLOT(cmd1_err(QString)), Qt::QueuedConnection);
    connect(cmd2, SIGNAL(info(QString)), this, SLOT(cmd2_info(QString)), Qt::QueuedConnection);
    connect(cmd2, SIGNAL(err(QString)), this, SLOT(cmd2_err(QString)), Qt::QueuedConnection);

    ui->listinfo2->hide();
    ui->start2->hide();
    ui->stop2->hide();

    ui->listinfo1->setGeometry( 30 , 20 ,  590   ,441 );
    this->move( 320 , 150 );

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_cmd1_ui()));
    timer->start(100);

    m_cnt1_temp = 0;
    m_cnt1 = 0;
    line_count = 0;

}


MainWindow::~MainWindow()
{

    delete ui;
    delete cmd1;
    delete cmd2;

}


void MainWindow::closeEvent(QCloseEvent *event)
{
    if (cmd1->freeable() && cmd2->freeable())
        event->accept();
    else {
        cmd1->cancel();
        cmd2->cancel();
        event->ignore();
    }
}


void MainWindow::cmd1_started()
{
    ui->stop1->setEnabled(true);
    ui->start1->setEnabled(false);
}

void MainWindow::cmd2_started()
{
    ui->stop2->setEnabled(true);
    ui->start2->setEnabled(false);
}

void MainWindow::cmd1_stopped()
{
    ui->stop1->setEnabled(false);
    ui->start1->setEnabled(true);
}

void MainWindow::cmd2_stopped()
{
    ui->stop2->setEnabled(false);
    ui->start2->setEnabled(true);
}


void MainWindow::wbuf_info(  void )
{

   char W_buf_str[ 300 ];

   Listinfo_W_pattern->clear();

    for(  int i = 0  ;  i <  (  buffer_length * 512 )/16 ; i++  )
      {

          //sprintf( W_buf_str  , "%04d: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X" , ( i * 16  )   ,  buf_W[  ( 16 * i )  + 1 ]        ,  buf_W[  ( 16 * i )  + 1 ]  , buf_W[  ( 16 * i )  + 2 ]  ,   buf_W[  ( 16 * i )  + 3 ]  ,    buf_W[  ( 16 * i )  + 4 ]   ,    buf_W[  ( 16 * i )  + 5 ]  ,  buf_W[  ( 16 * i )  + 6 ]  ,    buf_W[  ( 16 * i )  + 7 ] ,    buf_W[  ( 16 * i )  + 8 ] ,   buf_W[  ( 16 * i )  + 9 ]  ,   buf_W[  ( 16 * i )  + 10 ]  ,   buf_W[  ( 16 * i )  + 11 ]   ,  buf_W[  ( 16 * i )  + 12 ]  ,   buf_W[  ( 16 * i )  + 13 ]   ,   buf_W[  ( 16 * i )  + 14 ]  ,    buf_W[  ( 16 * i )  + 15 ]    ) ;

           sprintf( W_buf_str  , "%04d: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X" , ( i * 16  )   ,  buf_W[ ( 16 * i )]  ,  buf_W[  ( 16 * i )  + 1 ]  , buf_W[  ( 16 * i )  + 2 ]  ,   buf_W[  ( 16 * i )  + 3 ]  ,    buf_W[  ( 16 * i )  + 4 ]   ,    buf_W[  ( 16 * i )  + 5 ]  ,  buf_W[  ( 16 * i )  + 6 ]  ,    buf_W[  ( 16 * i )  + 7 ] ,    buf_W[  ( 16 * i )  + 8 ] ,   buf_W[  ( 16 * i )  + 9 ]  ,   buf_W[  ( 16 * i )  + 10 ]  ,   buf_W[  ( 16 * i )  + 11 ]   ,  buf_W[  ( 16 * i )  + 12 ]  ,   buf_W[  ( 16 * i )  + 13 ]   ,   buf_W[  ( 16 * i )  + 14 ]  ,    buf_W[  ( 16 * i )  + 15 ]    ) ;

           Listinfo_W_pattern->addItem( W_buf_str );

      }

    /*
    // ---------------------------------------------------------------------

    JUST.m_wbuf = JUST.get_wbuf( ( buffer_length * 512 )  );
    //JUST.m_rbuf = JUST.get_rbuf( ( buffer_length * 512 )  );

     for( int i = 0 ; i < ( buffer_length * 512 )   ; i++ )
       {
             printf(   "%X",     JUST.m_wbuf[ i ]    ) ;
       }

      printf( "\n ") ;

   // ------------------------------------------------------------------------
    */

}

void MainWindow::rbuf_info(  void )
{

    char R_buf_str[ 300 ];

    Listinfo_R_pattern->clear();

    for(  int i = 0  ;  i <  ( buffer_length * 512 )/16 ; i++  )
       {

          // sprintf( R_buf_str  , "%04d: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X" , ( i * 16  )   ,  buf_R[ ( 16 * i )]  ,  buf_R[  ( 16 * i )  + 1 ]  , buf_R[  ( 16 * i )  + 2 ]  ,   buf_R[  ( 16 * i )  + 3 ]  ,    buf_R[  ( 16 * i )  + 4 ]   ,    buf_R[  ( 16 * i )  + 5 ]  ,  buf_R[  ( 16 * i )  + 6 ]  ,    buf_R[  ( 16 * i )  + 7 ] ,    buf_R[  ( 16 * i )  + 8 ] ,   buf_R[  ( 16 * i )  + 9 ]  ,   buf_R[  ( 16 * i )  + 10 ]  ,   buf_R[  ( 16 * i )  + 11 ]   ,  buf_R[  ( 16 * i )  + 12 ]  ,   buf_R[  ( 16 * i )  + 13 ]   ,   buf_R[  ( 16 * i )  + 14 ]  ,    buf_R[  ( 16 * i )  + 15 ]    ) ;

          sprintf( R_buf_str  , "%04d: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X" , ( i * 16  )   ,  buf_R[ ( 16 * i )]  ,  buf_R[  ( 16 * i )  + 1 ]  , buf_R[  ( 16 * i )  + 2 ]  ,   buf_R[  ( 16 * i )  + 3 ]  ,    buf_R[  ( 16 * i )  + 4 ]   ,    buf_R[  ( 16 * i )  + 5 ]  ,  buf_R[  ( 16 * i )  + 6 ]  ,    buf_R[  ( 16 * i )  + 7 ] ,    buf_R[  ( 16 * i )  + 8 ] ,   buf_R[  ( 16 * i )  + 9 ]  ,   buf_R[  ( 16 * i )  + 10 ]  ,   buf_R[  ( 16 * i )  + 11 ]   ,  buf_R[  ( 16 * i )  + 12 ]  ,   buf_R[  ( 16 * i )  + 13 ]   ,   buf_R[  ( 16 * i )  + 14 ]  ,    buf_R[  ( 16 * i )  + 15 ]    ) ;

           Listinfo_R_pattern->addItem( R_buf_str );

       }

  /*
    // ---------------------------------------------------------------------

    //JUST.m_wbuf = JUST.get_wbuf( ( buffer_length * 512 )  );
    JUST.m_rbuf = JUST.get_rbuf( ( buffer_length * 512 )  );

     for( int i = 0 ; i < ( buffer_length * 512 )   ; i++ )
       {
               printf(   "%X",     JUST.m_rbuf[ i ]    ) ;

       }

      printf( "\n ") ;

   // ------------------------------------------------------------------------
   */


}


void MainWindow::cmd1_info(QString msg)
{

    m_msg_list1.append(msg);

    m_cnt1++;

    Parse_Msg( msg );

    if( m_save_log_checked == true )
    LOG( msg );

}


void MainWindow::update_cmd1_ui()
{
    if (m_cnt1 > 10000)
     {
          Listinfo->clear();
          m_cnt1 = m_msg_list1.length();
     }

    Listinfo->addItems( m_msg_list1 );
    m_msg_list1.clear();

   if( m_control_scroll == true ) // if run script true , run command input false
     Listinfo->scrollToBottom();

   wbuf_info();
   rbuf_info();

  if( m_cnt1 > m_cnt1_temp )
   {

       if( m_control_scroll == false  )
          Listinfo->scrollToBottom();

       m_cnt1_temp = m_cnt1;;

   }

}


void MainWindow::wheelEvent(QWheelEvent* event)
{

}


void MainWindow::cmd2_info(QString msg)
{

}

void MainWindow::cmd1_err(QString msg)
{

    Listinfo->addItem(msg);

    Parse_Msg( msg );

    if( m_save_log_checked == true )
      LOG( msg );

}

void MainWindow::cmd2_err(QString msg)
{

}


void MainWindow::on_CLose_clicked()
{
    this->close();
}

void MainWindow::on_stop1_clicked()
{

}

void MainWindow::on_start1_clicked()
{

}
