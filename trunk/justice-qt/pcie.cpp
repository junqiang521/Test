#include "hiredis/hiredis.h"
#include "pcie.h"
#include "ui_pcie.h"
#include <QDirModel>
#include <QScrollBar>
#include <QStandardItemModel>
#include <pcie.h>
#include <QFileDialog>
#include <QTimer>
#include <stdio.h>
#include <QLabel>
#include <QDialog>
#include <QTableView>
#include <QHeaderView>
#include <QPoint>
#include <QMessageBox>
#include <QThread>
#include <QtTest/QTest>
#include <QApplication>
#include <QException>
#include <QSaveFile>
#include <QFileSystemModel>
#include "time.h"
#include <QDebug>
#include <QFile>
#include <QObject>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QListWidget>
#include <QLayout>
#include <QWidget>
#include <QButtonGroup>
#include <QtGui>
#include <QUiLoader>
#include <stdio.h>
#include <unistd.h>
#include <QStatusBar>
#include <QDesktopWidget>
#include <signal.h>
#include <QUdpSocket>
#include <QTextStream>
#include <sys/stat.h>
#include <QThread>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "justice_cmd.h"
#include <QCloseEvent>
#include <assert.h>
#include <QStringList>
#include <mainwindow.h>
#include <justice_cmd.h>
#include <QWhatsThis>
#include <QMenuBar>
#include <QMouseEvent>
#include <QTextEdit>
#include <log_setting.h>
#include <buffer_setting.h>

char ArgV_P1[ 100 ] , ArgV_P2[ 100 ] , ArgV_P3[ 100 ];   // command input temp buffer
char FW_ver[ 200 ];
char DUT_name[ 50 ];
long DUT_capacity;
long MAX_LBA;
char Jdriver_ver[ 200 ];

// set justice.ini
//---------------------------------------
 QString  Product_tilte;
 QString  Script_default_path;
 QString  MAX_line_clear_form;
 QString  Power_Board_folder_path;
 QString  Set_default_buffer_length;
 QString  Set_DUT_PCIE_number;
 //--------------------------------------

QPushButton  *Run2;
QPushButton  *Stop2;


FILE *Log_file;
FILE *Input_log_file;
QString mPath ;

QStandardItemModel *pcie_s = new QStandardItemModel;

#define BUF_LEN  65536
#define SHARED_MEM_SIZE (64 * 1024 * 1024)

int  mode_sw;

// --------- time viable -------------
time_t time_start, time_end;
int  time_diff;
bool time_resert ;
long sec , minutes  , hours ,  days;
extern long sec_c , minutes_c  , hours_c , days_c ;
extern QLabel *Elapsed_c_label;
//------------------------------------

QString  filePath , file_name;

QListWidget  *Listinfo;
QTextEdit    *TextEdit_display_code;
QListWidget  *Listinfo_W_pattern;
QListWidget  *Listinfo_R_pattern;
QWidget      *Input_help;

QLabel *stat_model ;
QLabel *stat_capacity ;
QLabel *stat_FW_var ;
QLabel *stat_MAX_LBA;
QLabel *stat_driver_ver;
QLabel *stat_pcie_num;
QLabel *stat_status_1 ;

QStringList script_argv_str;
QString fileName_node ; // For node.js test script name

unsigned char cns;
bool timer_elapsed_enable;

char sch[ 100 ];        // connand line buffer
char *lc;
time_t t_now;
struct tm *tm_now;
bool TEST_STOP;
long GUI_string_line_count;
char MS[ 100 ];
char log_str[ 200 ];
char Input_log_str[ 200 ];
bool m_control_scroll;
int str_length , str_length_temp ;
bool log_mode_sw;  // script mode : 1 , input mode : 0
int process_ID;
bool command_input_mode;
char script_name[ 50 ];

extern int m_argc;
extern QString filePath_args;
extern QString serach_jdriver_str;  // string : lsmod grep | jdoor
extern int buffer_length;
extern bool m_save_log_checked;
extern bool m_modify_log_self_checked;
extern QString log_name_self;

void LOG( QString str );
void Save_log( char *str );
void Parse_Msg( QString str );

void loadSettings_KeyIn();
void SaveSettings_KeyIn();
QString  K_Test_count;
QString  K_Admin;
QString  K_OPcode;
QString  K_DW1;
QString  K_DW10;
QString  K_DW11;
QString  K_DW12;

bool Command_test_enable;
extern char buff[300];



justice_cmd  J;

PCIE::PCIE(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PCIE)
{

    ui->setupUi(this);


    // ----Read Justice .ini file -----------------------------
    loadSettings();
    this->setWindowTitle(  Product_tilte  );
    this->setGeometry( 100 , 40 , 1158 ,  820  );
   //----------------------------------------------------------

   dirModel = new QFileSystemModel(this);
   dirModel->setFilter(QDir::NoDotAndDotDot |
                       QDir::AllDirs);

   //------------------ LOad ini file -----------------------
   mPath = "";
   mPath = Script_default_path ;
   dirModel->setRootPath( mPath );
   //----------------------------------------------------------

   fileModel = new QFileSystemModel(this);
   fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
   fileModel->setRootPath(mPath);

   int max_line = 100;

       for(  int  i = 0 ; i < max_line ; i++ )
         {

             pcie_s ->setItem( i , 0 , new QStandardItem( "" ));
             pcie_s->item( i ,   0)->setTextAlignment( Qt::AlignCenter );

             pcie_s->setItem( i , 1  , new QStandardItem(( "" )));
             pcie_s ->item( i ,   1 )->setTextAlignment( Qt::AlignCenter );

             pcie_s->setItem( i , 2  , new QStandardItem(( "" )));
             pcie_s ->item( i , 2 )->setTextAlignment( Qt::AlignCenter );

             pcie_s ->setItem( i , 3  ,  new QStandardItem(( "" )));
             pcie_s->item( i ,   3)->setTextAlignment( Qt::AlignCenter );

             pcie_s ->setItem( i , 4  ,  new QStandardItem(( "" )));
             pcie_s->item( i , 3)->setTextAlignment( Qt::AlignCenter );

         }

  // -------------------------------------------

  QPushButton *Run2  = new QPushButton();
  QPushButton *Stop2 = new QPushButton();
  Run2->setParent( this );
  Stop2->setParent( this );
  Run2->setGeometry( 10 , 100 , 50 , 25  );
  Run2->setText( "Run");
  Stop2->setGeometry( 60 , 100 , 50 , 25 );
  Stop2->setText( "Stop");
  Run2->show();
  Stop2->show();
  Run2->setEnabled( 1 );
  Stop2->setEnabled( 0 );

  QObject::connect( Run2 , SIGNAL(clicked()) , this , SLOT( Run2_clicked() ) ) ;
  QObject::connect( Stop2 , SIGNAL(clicked()) , this , SLOT( Stop2_clicked() ) );

  Run2->hide();
  Stop2->hide();
  //--------------------------------------------



   timer_elapsed = new QTimer(this);
   QObject::connect( this->timer_elapsed , SIGNAL(timeout()) , this , SLOT( time_elapsed() )  )   ;
   //QObject::connect( timer_elapsed , SIGNAL(timeout()) , this , SLOT( time_elapsed() )  )   ;

   Listinfo = new QListWidget();
   TextEdit_display_code = new QTextEdit();
   Listinfo_R_pattern = new QListWidget();  // Read buffer windows
   Listinfo_W_pattern = new QListWidget();  // Write buffer windows  
   Input_help = new QWidget();

   //Listinfo_KeyTest = new QWidget();

   mode_sw = 1;

  // --------------------- Status Bar ------------------------

    sprintf( MS , " Model : %s"  , DUT_name  );
    stat_model = new QLabel( MS );
    stat_model->setAlignment( Qt::AlignLeft );

    stat_model->setLineWidth( 1 );
    stat_model->setMidLineWidth( 1 );

    DUT_capacity = ( (  MAX_LBA * 512 )  / 1024  / 1024 / 1024 );
    sprintf( MS , "  Capacity : %d GB"  , DUT_capacity   );
    stat_capacity = new QLabel( MS);
    stat_capacity->setAlignment( Qt::AlignLeft );

    stat_capacity->setLineWidth( 1 );
    stat_capacity->setMidLineWidth( 1 );
    sprintf( MS , "    Firmware ver : %s "  ,  FW_ver  );
    stat_FW_var = new QLabel( MS );
    stat_FW_var->setAlignment( Qt::AlignLeft );

    stat_FW_var->setLineWidth( 1 );
    stat_FW_var ->setMidLineWidth( 1 );

    sprintf( MS , " MAXLBA : %d"  ,  MAX_LBA );
    stat_MAX_LBA = new QLabel( MS );
    stat_MAX_LBA->setAlignment( Qt::AlignLeft );

    stat_MAX_LBA ->setLineWidth( 1 );
    stat_MAX_LBA->setMidLineWidth( 1 );

    sprintf( MS , " Driver ver : %s"  , Jdriver_ver );
    stat_driver_ver = new QLabel( MS );
    stat_driver_ver->setAlignment( Qt::AlignLeft );

    stat_driver_ver->setLineWidth( 1 );
    stat_driver_ver->setMidLineWidth( 1 );

    sprintf( MS , "  DUT : %d"  ,Set_DUT_PCIE_number.toInt()  );
    stat_pcie_num = new QLabel( MS );
    stat_pcie_num->setAlignment( Qt::AlignLeft );

    stat_pcie_num->setLineWidth( 1 );
    stat_pcie_num->setMidLineWidth( 1 );


    sprintf( MS   ,  "    Status : Stop"  );
    stat_status_1 = new QLabel(  MS  );
    stat_status_1->setAlignment( Qt ::AlignLeft);
    stat_status_1->setLineWidth( 1 );
    stat_status_1->setMidLineWidth( 1 );

    StatusBar= new QStatusBar();

    StatusBar->addWidget(   stat_model ,   28   );
    StatusBar->addWidget(   stat_FW_var ,  32   );
    StatusBar->addWidget(   stat_capacity, 25  );
    StatusBar->addWidget(   stat_MAX_LBA,  32   );
    StatusBar->addWidget(   stat_driver_ver , 35   );
    StatusBar->addWidget(   stat_pcie_num , 15   );
    StatusBar->addWidget(   stat_status_1 , 25 );  // Status : STOP , RUN
    StatusBar->setGeometry( 0 , 790 , 1100 , 50 );
    StatusBar->setParent( this );

    days = 0;
    hours = 0;
    minutes = 0;
    sec = 0;

    char MTS[ 100 ];
    sprintf( MTS , "Elapsed : Day %02d  : Hour %02d  : Min %02d  : Sec %02d", days, hours, minutes,  0);
    ui->label_13->setText(   MTS     );

   ui->label_6->hide();     // script name
   ui->lineEdit_2->hide();

    ui->pushButton_28->setText( "View");

   timer_elapsed_enable = 0;

   TEST_STOP = 0;

   // Initial GUI Display string count , if count more than 30000 , GUI clear all
   GUI_string_line_count = 0 ;

   Get_DUT_information() ;

   str_length = 0;

   m_control_scroll= true;

   log_mode_sw = 0;
   Start_Input_log(); // input log

   process_ID = J.m_wshmid; // PROCESS id
   command_input_mode = false;

   TextEdit_display_code->setWindowTitle( "Program windows" );
   Listinfo->setWindowTitle( "Log windows" );
   Listinfo_W_pattern->setWindowTitle( "Write buffer" );
   Listinfo_R_pattern->setWindowTitle(  "Read buffer" );

   // Set Menu
   QMenuBar* menu  = new QMenuBar( this );
   QMenu* mainMenu1 = new QMenu("    File     ");
   QMenu* mainMenu2 = new QMenu("    BUFFER      ");
   QMenu* mainMenu3 = new QMenu("    DUT     ");
   QMenu* mainMenu4 = new QMenu("    TOOL    ");
   QMenu* mainMenu5 = new QMenu("    HELP    ");
   QMenu* menu4C 	= new QMenu("Setting");

   // mainMenu1
    QString Menu_str;
    Menu_str = "New               ";
    QAction* action1A = mainMenu1->addAction( Menu_str.toStdString().c_str() );
    Menu_str = "Open              ";

    QAction* action1B = mainMenu1->addAction( Menu_str.toStdString().c_str() );

    Menu_str = "Save              "  ;
    QAction* action1D = mainMenu1->addAction( Menu_str.toStdString().c_str() );
    Menu_str = "Save As           "  ;
    QAction* action1E = mainMenu1->addAction( Menu_str.toStdString().c_str());
    mainMenu1->addSeparator();
    Menu_str = "Close             "  ;
    QAction* action1F = mainMenu1->addAction( Menu_str.toStdString().c_str() );
    Menu_str = "Edit              "   ;
    QAction* action1G = mainMenu1->addAction( Menu_str.toStdString().c_str() );

    QAction* action2A = mainMenu2->addAction( "Clear               " );
    QAction* action2B = mainMenu2->addAction( "Fill                " );
    QAction* action2C = mainMenu2->addAction( "Compare             " );
    QAction* action2D = mainMenu2->addAction( "Copy                " );
    QAction* action2E = mainMenu2->addAction( "Save Bin            " );

    QAction* action3A = mainMenu3->addAction( "Device Information       " );

    QAction* action4A = mainMenu4->addAction( "None                    " );
    QAction* action4B = mainMenu4->addAction( "None                    " );
    mainMenu4->addSeparator();
    mainMenu4->addMenu( menu4C );

    QAction* action4CA = menu4C->addAction( "Log setting                  " );
    QAction* action4CB = menu4C->addAction( "Buffer setting               " );
    QAction* action4CC = menu4C->addAction( "Power cycle setting          " );
    QAction* action4CD = menu4C->addAction( "Data base setting            " );

    QAction* action5A = mainMenu5->addAction( "CMD input               F1");
    QAction* action5B = mainMenu5->addAction( "About Justice            ");

   menu->addMenu( mainMenu1 );
   menu->addMenu( mainMenu1 );
   menu->addMenu( mainMenu2 );
   menu->addMenu( mainMenu2 );
   menu->addMenu( mainMenu3 );
   menu->addMenu( mainMenu4 );
   menu->addMenu( mainMenu5 );

   connect( action1A,  SIGNAL(triggered()),this,SLOT( slot_menu_action_NEW()  ));
   connect( action1B , SIGNAL(triggered()),this,SLOT( slot_menu_action_OPEN()  ));

   connect( action1D , SIGNAL(triggered()),this,SLOT( slot_menu_action_SAVE()  ));
   connect( action1E , SIGNAL(triggered()),this,SLOT( slot_menu_action_SAVEAS()) );
   connect( action1F , SIGNAL(triggered()),this,SLOT( slot_menu_action_CLOSE()) );
   connect( action1G , SIGNAL(triggered()),this,SLOT( slot_menu_action_EXIT()) );

   connect( action2A,  SIGNAL(triggered()),this,SLOT( slot_menu_action_BUFFER_CLEAR()  ));
   connect( action2B , SIGNAL(triggered()),this,SLOT( slot_menu_action_BUFFER_FILL()  ));
   connect( action2C , SIGNAL(triggered()),this,SLOT( slot_menu_action_BUFFER_COMPARE_()  ));
   connect( action2D , SIGNAL(triggered()),this,SLOT( slot_menu_action_BUFFER_COPY_()  ));
   connect( action2E , SIGNAL(triggered()),this,SLOT( slot_menu_action_BUFFER_SAVE_BIN()) );

   connect( action3A,  SIGNAL(triggered()),this,SLOT( slot_menu_action_TEST_STATUS()  ));

   connect( action4A,  SIGNAL(triggered()),this,SLOT( slot_menu_action_TOOLA()  ));
   connect( action4B , SIGNAL(triggered()),this,SLOT( slot_menu_action_TOOLB()  ));

   connect( action4CA , SIGNAL(triggered()),this,SLOT( slot_menu_action_TOOLLOG_SETTING()  ));
   connect( action4CB , SIGNAL(triggered()),this,SLOT( slot_menu_action_TOOL_BUFFER_SETTING()  ));
   connect( action4CC , SIGNAL(triggered()),this,SLOT( slot_menu_action_TOOL_POWER_SETTING()  ));
   connect( action4CD, SIGNAL(triggered()),this,SLOT(  slot_menu_action_TOOL_DATABASE_SETTING()  ));

   connect( action5A,  SIGNAL(triggered()),this,SLOT( slot_menu_action_HELP_COMINPUT_F1()  ));
   connect( action5B , SIGNAL(triggered()),this,SLOT( slot_menu_action_HELP_ABOUT_JUSTICE() ));

   Listinfo->setGeometry( 100  , 200 , 500 , 515 );
   Listinfo->hide();

   TextEdit_display_code->setGeometry( 135 + ( 520 ) , 200   ,  500   , 515 );
   TextEdit_display_code->hide();

   ui->lineEdit->setGeometry( 890 + 50  , 728 , 185 , 27   );            // command input edit
   ui->CDM_input_label->setGeometry( 695 + 150 +20  , 730 , 71 , 20  );  // CDM input label

   m_save_log_checked = true;
   m_modify_log_self_checked = false;


   QFont font_11( "Courier New", 11 , QFont::Normal ) ;
   Listinfo->setFont( font_11 );

   QFont font_9( "Courier New", 9 , QFont::Normal ) ;
   Listinfo_W_pattern->setFont( font_9 );
   Listinfo_R_pattern->setFont( font_9 );


   K_test.setWindowTitle( "Justice Command test window"    );
   K_test.setGeometry(  310 , 300 , 487 , 380  );



}



void PCIE::closeEvent(QCloseEvent *event)
{

    QFile Fout( "Running.Jlog" );

    if(Fout.exists())
      remove("Running.Jlog");

    Listinfo_W_pattern->close();
    Listinfo_R_pattern->close();
    TextEdit_display_code->close();
    Listinfo->close();

    event->accept();

}

void PCIE::slot_menu_action_NEW()
{

    this->setWindowFlags( Qt::WindowStaysOnBottomHint );
    this->show();

    TextEdit_display_code->setGeometry( ( 190) , 210 ,  1000   , 515 );
    TextEdit_display_code->show();
    TextEdit_display_code->clear();
    QDir dir;
    QString Path_str = dir.absolutePath() + "/New.js";
    fileName_node =  Path_str;

    sprintf( MS , "%s %s" ,"Program windows" ,  "New file" );
    TextEdit_display_code->setWindowTitle( MS );

}


void PCIE::slot_menu_action_OPEN()
{

    this->setWindowFlags( Qt::WindowStaysOnBottomHint );
    this->show();

    TextEdit_display_code->setWindowFlags( 0 );
    Listinfo->setWindowFlags( 0 );
    Listinfo_W_pattern->setWindowFlags( 0 );
    Listinfo_R_pattern->setWindowFlags( 0 );

   fileName_node = QFileDialog::getOpenFileName( this, tr("Open File"),  filePath  , tr("Images (*.js *)"));

   if( strlen( fileName_node.toStdString().c_str() ) == 0 )     
       return;
   else
       Listinfo->setParent( 0 );

   LoadScriptContext(  fileName_node );

   m_argc == 1 ; // active script test

   char str_name_temp[ 100 ] , str_name[ 100 ];

   sprintf( str_name_temp , "%s" , fileName_node.toStdString().c_str() );

   int j = 0 ;


   for( int i = strlen( str_name_temp ) - 1; i >= 0 ;   i--  )
    {

        str_name[ j ]  = str_name_temp[ i  ] ;
        j++;

       if( str_name_temp[ i ] == '/' )
         {
            str_name[ j - 1  ] =  '\0';
            str_name_temp[ i ] =  '\0';
            break;
         }
    }

   int M = 0;

   for( int k = j - 2  ;  k >= 0 ; k-- )
    {
       script_name[ M ]  = str_name[ k ];
       sprintf( MS , "%C" ,     script_name[ M ]  );
       //QMessageBox::about(this,"Active 3",    MS  );
       M++;

    }

   script_name[ M ] = '\0';
   sprintf( MS , "Script name : %s" ,  script_name );
   ui->name_label->setText( MS );

   Listinfo->setGeometry( 100 , 200 ,  500  , 515 );

   Listinfo_W_pattern->setGeometry( ( 900 - 28 ) , 200 , ( 380 + 14 ) , 300 );
   Listinfo_R_pattern->setGeometry( ( 900 - 28 ) , 500 + 30 , ( 380 + 14 ) , 300 );



   Listinfo_W_pattern->show();
   Listinfo_R_pattern->show();
   Listinfo->show();

   TextEdit_display_code->move(QApplication::desktop()->screen()->rect().center() - Listinfo->rect().center());
   TextEdit_display_code->setGeometry(  150 , 200  ,  1050 , 515  );
   TextEdit_display_code->show();


   sprintf( MS , "%s  %s" ,"Program windows" ,  fileName_node.toStdString().c_str() );
   TextEdit_display_code->setWindowTitle( MS );

   sprintf( MS , "%s  %s" ,"Log windows" ,  fileName_node.toStdString().c_str() );
   Listinfo->setWindowTitle( MS );



}


void PCIE::slot_menu_action_SAVE()
{

    if( fileName_node == NULL   )
     {
        QMessageBox::information( this, tr("Justice"), tr(  "Please select JS file          ") );
        return;
     }

    sprintf( MS , "rm %s" ,  fileName_node.toStdString().c_str() );
    system( MS );

          QFile outfile;
          QDir dir( fileName_node  );

            outfile.setFileName(  dir.absoluteFilePath(   fileName_node  )  );

           if(  outfile.open(QIODevice:: ReadWrite| QIODevice::Text) )
              {
                   QTextStream out(&outfile);
                   out <<  TextEdit_display_code->toPlainText()<< endl;
                   //QMessageBox::information(this,   "Justice"  ,   "               Save script ok                                 "   , QMessageBox::Ok );
              }
            else
              {

                  TextEdit_display_code->hide();
                  Listinfo->hide();
                  Listinfo_W_pattern->hide();
                  Listinfo_R_pattern->hide();

                  QMessageBox::warning( this, tr("Justice"), tr("          Save script fail!!                    ") );

                  TextEdit_display_code->show();
                  Listinfo->show();
                  Listinfo_W_pattern->show();
                  Listinfo_R_pattern->show();

                  return;
              }

}



void PCIE::slot_menu_action_SAVEAS()
{

    if( fileName_node == NULL   )
     {
        QMessageBox::information( this, tr("Justice"), tr(  "Please select JS file          ") );
        return;
     }

    TextEdit_display_code->hide();
    Listinfo->hide();
    Listinfo_W_pattern->hide();
    Listinfo_R_pattern->hide();

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save as file"),  filePath  ,  tr("Images (*.js *)"));
    QFile outfile;
    QDir dir( filePath );
    outfile.setFileName(  dir.absoluteFilePath( fileName )  );

    outfile.open(QIODevice:: ReadWrite| QIODevice::Text);

    QTextStream out(&outfile);
    out <<  TextEdit_display_code->toPlainText()<< endl;


    if( strlen( fileName_node.toStdString().c_str() ) == 0 )
        return;

    TextEdit_display_code->show();

}


void PCIE::slot_menu_action_CLOSE()
{

    TextEdit_display_code->close();
    Listinfo->close();
    Listinfo_W_pattern->close();
    Listinfo_R_pattern->close();

}


void PCIE::slot_menu_action_EXIT()
{

    TextEdit_display_code->close();
    Listinfo->close();
    Listinfo_W_pattern->close();
    Listinfo_R_pattern->close();
    this->close();
    K_test.close();



}


void PCIE::slot_menu_action_BUFFER_CLEAR()
{



}

void PCIE::slot_menu_action_BUFFER_FILL()
{



}

void PCIE::slot_menu_action_BUFFER_COMPARE_()
{



}

void PCIE::slot_menu_action_BUFFER_COPY_()
{



}

void PCIE::slot_menu_action_BUFFER_SAVE_BIN()
{


}

void PCIE::slot_menu_action_TEST_STATUS()
{

   S_dev.setWindowFlags( Qt::WindowStaysOnTopHint );
   S_dev.move( QApplication::desktop()->screen()->rect().center() - S_dev.rect().center());
   S_dev.show();

}


void PCIE::slot_menu_action_TOOLA()
{


}

void PCIE::slot_menu_action_TOOLB()
{


}


void PCIE::slot_menu_action_TOOLLOG_SETTING()
{
  this->show();
  Log_s.setWindowFlags( Qt::WindowStaysOnTopHint );
  Log_s .move( QApplication::desktop()->screen()->rect().center() - Log_s.rect().center());
  Log_s.show();

}

void PCIE::slot_menu_action_TOOL_BUFFER_SETTING()
{
   this->show();
   Buffer_s.setWindowFlags( Qt::WindowStaysOnTopHint  );
   Buffer_s.move( QApplication::desktop()->screen()->rect().center() - Buffer_s.rect().center());
   Buffer_s.show();

}

void PCIE::slot_menu_action_TOOL_POWER_SETTING()
{


}

void PCIE::slot_menu_action_TOOL_DATABASE_SETTING()
{
  //QMessageBox::about(this,"Active 2","Active 4CD - Edit ");

}

void PCIE::slot_menu_action_HELP_COMINPUT_F1()
{

    QString com_cls , com_str_id , com_str_gfea , com_str_glog , com_str_read ,com_str_write ;
    QString com_str_pci , com_str_ctrl , com_str_cfgpmcap , com_str_cfgmsicap , com_str_cfgpxcap , com_str_cfgmsixcap , com_str_rread;
    QString com_str_girq , com_str_sirq;

    com_cls =             " 0) cls : Clear log windows all message\n";
    com_str_id =          " 1) identify | idfy : Para 0( 0 : namespace | 1 : controller ) \n" ;
    com_str_gfea =        " 2) gfea : Para 0( 1 - 11 ) \n";
    com_str_glog =        " 3) glog : Para 0( 1 - 255 ) , Para 1( 1 - 3 ) ) \n";

    com_str_read =        " 4) read : Para 0( LBA( 0 - MAXLBA ) ) , Para 1( Length( 1 - 255 ) ) \n";
    com_str_write =       " 5) write : Para 0( LBA( 0 - MAXLBA ) ) , Para 1( Length( 1 - 255 ) ) , Para 2( Pattern ) \n";

    com_str_pci =         " 6) gpci : Para 0( Null ) \n";

    com_str_cfgpmcap =    " 7) cfgpmcap : Para 0( Null ) \n";
    com_str_cfgmsicap =   " 9) cfgmsicap : Para 0( NUll ) \n";
    com_str_cfgpxcap =    " 9) cfgpxcap : Para 0( Null ) \n";
    com_str_cfgmsixcap =  " a) cfgmsixcap : Para 0( Null ) \n";

    com_str_rread      =  " b) rread : Para 0( 0 , 8 , 12 , 16 , 20 , 28 , 32 , 36 , 40 , 48 , 56 , 60  ) \n";

    com_str_ctrl =        " c) gctrl : Para 0( NULL ) \n";

    com_str_sirq =        " d) sirq : Para 0( 1 - 3 ) \n";
    com_str_girq =        " e) girq : Para 0( NUll ) \n";

    QString help_str;
    help_str = com_cls + com_str_id + com_str_gfea + com_str_glog + com_str_read + com_str_write +
    com_str_pci + com_str_ctrl + com_str_rread + com_str_cfgpmcap + com_str_cfgmsicap + com_str_cfgpxcap+ com_str_cfgmsixcap + com_str_girq + com_str_sirq;

    Input_help->setWindowTitle( " Command input help  "    );
    Input_help->setGeometry(  300 , 300 , 600 , 300  );

    QHBoxLayout* layout_help = new QHBoxLayout;
    Input_help->setLayout(layout_help);
    QLabel* Help_label = new QLabel;
    Help_label->setText( "Command input type :  \n\n" + help_str + "\n\n\n" +  "                                ( Press F1 key to open help window , F2 key to close ) "  );

    layout_help->addWidget( Help_label );
    Input_help->move(QApplication::desktop()->screen()->rect().center() - Input_help->rect().center());
    Input_help->setWindowFlags( Qt::WindowStaysOnTopHint );

    Input_help->show();
    ui->lineEdit->activateWindow();

}


void PCIE::slot_menu_action_HELP_ABOUT_JUSTICE()
{

}


void PCIE :: change( void )
{

    m_control_scroll= false;

}


void PCIE :: keyPressEvent( QKeyEvent * event )
{



  if ( event->key() == Qt::Key_F1 )
    {

        QString com_cls , com_str_id , com_str_gfea , com_str_glog , com_str_read ,com_str_write ;
        QString com_str_pci , com_str_ctrl , com_str_cfgpmcap , com_str_cfgmsicap , com_str_cfgpxcap , com_str_cfgmsixcap , com_str_rread;
        QString com_str_girq , com_str_sirq;

        com_cls =             " 0) cls : Clear log windows all message\n";
        com_str_id =          " 1) identify | idfy : Para 0( 0 : namespace | 1 : controller ) \n" ;
        com_str_gfea =        " 2) gfea : Para 0( 1 - 11 ) \n";
        com_str_glog =        " 3) glog : Para 0( 1 - 255 ) , Para 1( 1 - 3 ) ) \n";

        com_str_read =        " 4) read : Para 0( LBA( 0 - MAXLBA ) ) , Para 1( Length( 1 - 255 ) ) \n";
        com_str_write =       " 5) write : Para 0( LBA( 0 - MAXLBA ) ) , Para 1( Length( 1 - 255 ) ) , Para 2( Pattern ) \n";

        com_str_pci =         " 6) gpci : Para 0( Null ) \n";

        com_str_cfgpmcap =    " 7) cfgpmcap : Para 0( Null ) \n";
        com_str_cfgmsicap =   " 9) cfgmsicap : Para 0( NUll ) \n";
        com_str_cfgpxcap =    " 9) cfgpxcap : Para 0( Null ) \n";
        com_str_cfgmsixcap =  " a) cfgmsixcap : Para 0( Null ) \n";

        com_str_rread      =  " b) rread : Para 0( 0 , 8 , 12 , 16 , 20 , 28 , 32 , 36 , 40 , 48 , 56 , 60  ) \n";

        com_str_ctrl =        " c) gctrl : Para 0( NULL ) \n";

        com_str_sirq =        " d) sirq : Para 0( 1 - 3 ) \n";
        com_str_girq =        " e) girq : Para 0( NUll ) \n";

        QString help_str;
        help_str = com_cls + com_str_id + com_str_gfea + com_str_glog + com_str_read + com_str_write +
        com_str_pci + com_str_ctrl + com_str_rread + com_str_cfgpmcap + com_str_cfgmsicap + com_str_cfgpxcap+ com_str_cfgmsixcap + com_str_girq + com_str_sirq;

        Input_help->setWindowTitle( " Command input help  "    );
        Input_help->setGeometry(  300 , 300 , 600 , 300  );

        QHBoxLayout* layout_help = new QHBoxLayout;
        Input_help->setLayout(layout_help);
        QLabel* Help_label = new QLabel;
        Help_label->setText( "Command input type :  \n\n" + help_str + "\n\n\n" +  "                                ( Press F1 key to open help window , F2 key to close ) "  );
        layout_help->addWidget( Help_label );
        Input_help->move(QApplication::desktop()->screen()->rect().center() - Input_help->rect().center());
        Input_help->setWindowFlags( Qt::WindowStaysOnTopHint );

        Input_help->show();

        ui->lineEdit->activateWindow();

      }


  if ( event->key() == Qt::Key_F2 )
     {

        Input_help->close();

     }


 if (  event->key() ==  Qt::Key_Control   )
     {

       if( event->key() ==  Qt::Key_1  )
        QMessageBox::information( this,   "Hot_kEY"  ,  "Ctrl + N"   , QMessageBox:: Warning);

     }


  if ( event->key() == Qt::Key_F3 )
     {


     }


}



bool PCIE:: Auto_argv_run( void )
{

  Listinfo_W_pattern->setWindowFlags( 0 );
  Listinfo_R_pattern->setWindowFlags( 0 );
  TextEdit_display_code->setWindowFlags( 0 );

  this->setWindowFlags( Qt::WindowStaysOnBottomHint );
  this->show();

  Listinfo -> setParent( NULL );

  Listinfo->show();
  TextEdit_display_code->show();
  Listinfo_W_pattern->show();
  Listinfo_R_pattern->show();
  Listinfo->setGeometry( 100 , 200 ,  500  , 515 );
  TextEdit_display_code->setGeometry(  570 , 200 ,  500   , 515 );

  Listinfo_W_pattern->setGeometry( ( 900 - 28 ) , 200 , ( 380 + 14 ) , 300 );
  Listinfo_R_pattern->setGeometry( ( 900 - 28 ) , 500 + 30 , ( 380 + 14 ) , 300 );



  TextEdit_display_code->setStyleSheet( "border: 1px solid orange" );
  Listinfo->setStyleSheet(  "border: 1px solid orange");
  Listinfo_W_pattern->setStyleSheet(   "border: 1px solid orange");
  Listinfo_R_pattern->setStyleSheet(  "border: 1px solid orange" );

  command_input_mode = false;

  log_mode_sw = 1;
  m_control_scroll = true;

  if( m_save_log_checked == true )
   Start_Record_log();

  //--------------------------------------------------------------------------------------

  fileName_node = filePath_args;

  LoadScriptContext(  fileName_node );

  m_argc == 1 ; // active script test

  char str_name_temp[ 100 ] , str_name[ 100 ];

  sprintf( str_name_temp , "%s" , fileName_node.toStdString().c_str() );

  int j = 0 ;


  for( int i = strlen( str_name_temp ) - 1; i >= 0 ;   i--  )
   {

       str_name[ j ]  = str_name_temp[ i  ] ;
       j++;

      if( str_name_temp[ i ] == '/' )
        {
           str_name[ j - 1  ] =  '\0';
           str_name_temp[ i ] =  '\0';
           break;
        }
   }

  int P = 0;

  for( int k = j - 2  ;  k >= 0 ; k-- )
   {
      script_name[ P ]  = str_name[ k ];
      sprintf( MS , "%C" ,     script_name[ P ]  );
      //QMessageBox::about(this,"Active 3",    MS  );
      P++;

   }

  script_name[ P ] = '\0';
  sprintf( MS , "Script name : %s" ,  script_name );
  ui->name_label->setText( MS );

 //--------------------------------------------------------------------------------------

   M.cmd1->execute(QString( filePath_args ), QStringList() );

 TEST_STOP = 0;

 timer_elapsed->stop();

 time_resert = false ;

 timer_elapsed_enable = 1;

 stat_status_1->setText( "    Status : Run" );

 if( time_resert == false )
   {
      time(&time_start);
      time_resert = true ;
   }

 days = 0;
 hours = 0;
 minutes = 0;
 sec = 0;

 timer_elapsed->stop();
 timer_elapsed->start( 1000 );   // 1 second

 mode_sw = 1;

 ui->Run1->setEnabled( false );
 ui->Stop1->setEnabled( true );

 //Run2->setEnabled( false );
 //Stop2->setEnabled( true );

 // if message less 5 chara and elapsed time more than 5 second , system stop and elpased time stop
 str_length = 0;
 str_length_temp = 0;


}


bool PCIE:: install_jdriver( void )
{

   //---- check driver exist , Install Jdriver

    char com_str[ 100 ];
    sprintf(  com_str , "%s" , "modprobe  -r  nvme" );
    system( com_str );

    QString fileName;

    fileName = "jdoor.ko";

    QFileInfo checkFile( fileName );

    if( checkFile.exists() && checkFile.isFile() )
      {
          sprintf(  com_str , "%s" , "insmod jdoor.ko" );
          system( com_str );
      }
    else
      {
        sprintf( MS , "%s" , "      Can't find jdoor.ko driver             "  );
        QMessageBox::information( this,   "Error"  , MS   , QMessageBox:: Warning);
        return false;
      }


    fileName = "nvme.ko";

    if( checkFile.exists() && checkFile.isFile() )
      {
          sprintf(  com_str , "%s" , "insmod nvme.ko" );
          system( com_str );

      }
    else
      {
        sprintf( MS , "%s" , "      Can't find nvme.ko driver             "  );
        QMessageBox::information( this,   "Error"  , MS   , QMessageBox::Warning );
        return false;
      }

    return true;

}



bool PCIE::Get_DUT_information( void )
{

    ArgV_P1[ 0 ] = '0';  // default set nvme0on1
    ArgV_P1[ 1 ] ='\0';

    script_argv_str.clear();
    script_argv_str << "Script" <<  ArgV_P1;
    file_name = "ID.js";

    M.cmd1->execute(QString( file_name ),  script_argv_str ); // 0727 - justice_cmd class

    TEST_STOP = 0;

    timer_elapsed->stop();

    time_resert = false ;

    timer_elapsed_enable = 1;

    stat_status_1->setText( "    Status : Run" );

    if( time_resert == false )
      {
         time(&time_start);
         time_resert = true ;
      }

    days = 0;
    hours = 0;
    minutes = 0;
    sec = 0;

    timer_elapsed->stop();
    timer_elapsed->start( 200 );

    mode_sw = 1;

   return true;
}


bool PCIE::Justice_Command_input( QString filePath_com , QStringList Argv_str  )
{

    M.cmd1->execute(QString( filePath_com ), Argv_str ); // 0727 - justice_cmd class

    Listinfo->addItem( "" );

    mode_sw = 1;

    return true;
}


bool PCIE::Get_Driver_version( void )
{
    ArgV_P1[ 0 ] = '0';  // default set nvme0on1
    ArgV_P1[ 1 ] ='\0';

    script_argv_str.clear();
    script_argv_str << "Script" <<  ArgV_P1;

    //file_name = "/home/justice/justice-script/testcase/driver.js";
    file_name = "/home/justice/justice-script/command/driver.js";

    M.cmd1->execute(QString( file_name ),  script_argv_str ); // 0727 - justice_cmd class

    TEST_STOP = 0;

    timer_elapsed->stop();

    time_resert = false ;

    timer_elapsed_enable = 1;

    stat_status_1->setText( "    Status : Run" );

    if( time_resert == false )
      {
         time(&time_start);
         time_resert = true ;
      }

    days = 0;
    hours = 0;
    minutes = 0;
    sec = 0;

    timer_elapsed->stop();
    timer_elapsed->start( 1000 );   // 1 second

    mode_sw = 1;

    return true;
}



void PCIE::on_label_13_linkActivated( const QString &link )
{


}

void PCIE::on_pushButton_28_clicked()
{

    QFont font( "Courier New", 11 , QFont::Normal ) ;
    Listinfo->setFont( font );

    this->show();
    TextEdit_display_code->setWindowFlags( Qt::WindowStaysOnTopHint );
    Listinfo->setWindowFlags( Qt::WindowStaysOnTopHint );
    Listinfo_W_pattern->setWindowFlags( Qt::WindowStaysOnTopHint );
    Listinfo_R_pattern->setWindowFlags( Qt::WindowStaysOnTopHint );

    Listinfo -> setParent( 0 );

    if( mode_sw == 0 )
      {

        Listinfo->setGeometry( 100 , 200 ,  500  , 515 );
        Listinfo->show();

        TextEdit_display_code->setGeometry(  570 , 200 ,  500   , 515 );
        TextEdit_display_code->show();


        Listinfo_W_pattern->setGeometry( ( 900 - 28 ) , 200 , ( 380 + 14 ) , 300 );
        Listinfo_R_pattern->setGeometry( ( 900 - 28 ) , 500 + 30 , ( 380 + 14 ) , 300 );

        Listinfo_W_pattern->show();
        Listinfo_R_pattern->show();



        Listinfo->setGeometry( 100 , 200 ,  500  , 515 );
        Listinfo->show();

        TextEdit_display_code->setGeometry(  570 , 200 ,  500   , 515 );
        TextEdit_display_code->show();


        Listinfo_W_pattern->setGeometry( ( 900 - 28 ) , 200 , ( 380 + 14 ) , 300 );
        Listinfo_R_pattern->setGeometry( ( 900 - 28 ) , 500 + 30 , ( 380 + 14 ) , 300 );


        Listinfo_W_pattern->show();
        Listinfo_R_pattern->show();

        mode_sw = 1;

      }
    else if( mode_sw == 1 )
      {

        TextEdit_display_code->move(QApplication::desktop()->screen()->rect().center() - TextEdit_display_code->rect().center());
        TextEdit_display_code->setGeometry( 150 , 210 ,  1050 , 515 );
        TextEdit_display_code->show();

        Listinfo_W_pattern->hide();
        Listinfo_R_pattern->hide();
        Listinfo->hide();

        mode_sw = 2;

      }
    else if( mode_sw == 2 )
      {

        Listinfo->move(QApplication::desktop()->screen()->rect().center() - Listinfo->rect().center());
        Listinfo->setGeometry(  150  , 210 ,  1050  , 515 );
        Listinfo->show();

        TextEdit_display_code->hide();
        Listinfo_W_pattern->hide();
        Listinfo_R_pattern->hide();

        mode_sw = 0;

     }

}


PCIE::~PCIE()
{

    K_test.close();

    delete ui;

}


void PCIE::LoadScriptContext( QString script_file_name  )
{


     QString file_Name = script_file_name;

     if (!file_Name.isEmpty())
        {
           QFile file( file_Name );

           if (file.open(QFile::ReadOnly | QFile::Text))
             TextEdit_display_code->setPlainText(file.readAll());

        }

}


void PCIE::on_lineEdit_cursorPositionChanged( int arg1, int arg2)
{

}


// command line - argv 0 -3
void PCIE::P_parameter( char *str_loc ,  int cmd_len , int  com_num )
{

    int k = 0;
    int sp_count = 0;

    memset(  ArgV_P1 , NULL , sizeof(  ArgV_P1 ) );
    memset( ArgV_P2 , NULL , sizeof( ArgV_P2 )  );
    memset( ArgV_P3 , NULL , sizeof( ArgV_P3 )  );

    if( com_num == 1 )
        {

                for( int i =  cmd_len  ; i < 150 ; i++ )
                   {

                       if(  ( str_loc[ i ]  != ' ' ) &&  ( str_loc[ i ]  != NULL )  && ( sp_count == 0 )  )
                           {

                               for( int j = i ; j < 100 ; j++ )
                                 {


                                         if(  sp_count == 0 )
                                             {
                                                  ArgV_P1[ k ]  = str_loc[ j ]   ;
                                                  k++;

                                                if(  ( str_loc[  j  ]  == '\0 ')  /*&&  ( str_loc[ i ]  != NULL )*/  )
                                                   {
                                                        ArgV_P1[ k ] = '\0';


                                                        i = j  ;
                                                        k = 0;
                                                        sp_count++;


                                                    }

                                           }


                                  }

                         }

               }

   }

    if( com_num == 2 )
        {

                // dump , 0 , 1, 2
                for( int i =  cmd_len  ; i < 150 ; i++ )
                   {

                       if(  ( str_loc[ i ]  != ' ' ) &&  ( str_loc[ i ]  != NULL )  && ( sp_count == 0 )  )
                           {

                               for( int j = i ; j < 100 ; j++ )
                                 {


                                         if(  sp_count == 0 )
                                             {
                                                  ArgV_P1[ k ]  = str_loc[ j ]   ;
                                                  k++;

                                                if(  ( str_loc[  j  ]  == ' ')  /*&&  ( str_loc[ i ]  != NULL )*/  )

                                                   {
                                                         ArgV_P1[ k ] = '\0';

                                                        i = j  ;
                                                        k = 0;
                                                        sp_count++;


                                                    }

                                              }

                                   }

                        }


                        if(  ( str_loc[ i ]  != ' ' ) &&  ( str_loc[ i ]  != NULL )  && ( sp_count == 1 )  )
                           {



                               for( int j = i ; j < 100 ; j++ )
                                 {

                                         if(  sp_count == 1 )
                                             {
                                                 ArgV_P2[ k ]  = str_loc[ j ]   ;
                                                  k++;

                                                  if(  ( str_loc[  j  ]  == '\0' )  ||   ( str_loc[ i ]  == ' ' )  )
                                                   {
                                                        ArgV_P2[ k ] = '\0';

                                                        i = j  ;
                                                        k = 0;
                                                        sp_count++;
                                                        //break;

                                                    }

                                             }

                                 }

                        }

              }

}


    if( com_num == 3 )
        {

        int k = 0;
        int sp_count = 0;

        memset(  ArgV_P1 , NULL , sizeof( ArgV_P1 ) );
        memset( ArgV_P2 , NULL , sizeof( ArgV_P2 )  );
        memset( ArgV_P3 , NULL , sizeof( ArgV_P3 )  );

                    // dump , 0 , 1, 2, 3
                    for( int i =  cmd_len  ; i < 150 ; i++ )
                       {

                           if(  ( str_loc[ i ]  != ' ' ) &&  ( str_loc[ i ]  != NULL )  && ( sp_count == 0 )  )
                               {

                                   for( int j = i ; j < 100 ; j++ )
                                     {

                                             if(  sp_count == 0 )
                                                 {
                                                      ArgV_P1[ k ]  = str_loc[ j ]   ;
                                                      k++;

                                                     if(  ( str_loc[  j  ]  == ' ')  /*&&  ( str_loc[ i ]  != NULL )*/  )
                                                       {
                                                             ArgV_P1[ k ] = '\0';

                                                            i = j  ;
                                                            k = 0;
                                                            sp_count++;


                                                        }

                                                 }

                                        }

                              }


                            if(  ( str_loc[ i ]  != ' ' ) &&  ( str_loc[ i ]  != NULL )  && ( sp_count == 1 )  )
                               {

                                   for( int j = i ; j < 100 ; j++ )
                                     {

                                             if(  sp_count == 1 )
                                                 {
                                                     ArgV_P2[ k ]  = str_loc[ j ]   ;
                                                      k++;

                                                      if( ( str_loc[  j  ]  == ' ' )   /*&&  ( str_loc[ i ]  != NULL ) */  )
                                                       {
                                                            ArgV_P2[ k ] = '\0';
                                                           //QMessageBox::information( this,   "P2"  , P2  , QMessageBox::Ok);
                                                            i = j  ;
                                                            k = 0;
                                                            sp_count++;


                                                        }

                                                  }


                                      }

                            }


                           if(  ( str_loc[ i ]  != ' ' ) &&   ( str_loc[ i ]  != NULL )  && ( sp_count == 2 )  )
                               {

                                   for( int j = i ; j < 100 ; j++ )
                                     {

                                             if(  sp_count == 2 )
                                                 {
                                                     ArgV_P3[ k ]  =str_loc[ j ]   ;
                                                      k++;

                                                     if(  ( str_loc[  j  ]  == '\0' )  /* &&   ( str_loc[ i ]  != NULL )*/   )
                                                       {
                                                            ArgV_P3[ k ] = '\0';
                                                           //QMessageBox::information( this,   "P3"  , P3  , QMessageBox::Ok);
                                                            i = j ;
                                                            k = 0;
                                                            sp_count++;


                                                        }

                                                 }

                                       }

                               }

                        }

              }


             // QMessageBox::information( this,   "P1"  , P1  , QMessageBox::Ok);
             // QMessageBox::information( this,   "P2"  , P2  , QMessageBox::Ok);
             // QMessageBox::information( this,   "P3"  , P3  , QMessageBox::Ok);

}


// Command input function
void PCIE::on_lineEdit_returnPressed()
{

    Command_test_enable = 0;

    QFont font_11( "Courier New", 11 , QFont::Normal ) ;
    Listinfo->setFont( font_11 );

    M.m_cnt1 = 0;
    M.m_cnt1_temp = 0;
    M.m_msg_list1.clear();

    Listinfo->setParent( this );
    Listinfo->setGeometry( ( 45 - 4 )  ,  100 ,  1079   , 570 );
    Listinfo->show();

    log_mode_sw = 0;
    command_input_mode = true;
    m_control_scroll = false;

    M.m_cnt1 = 0;
    M.m_cnt1_temp = 0;
    M.m_msg_list1.clear();

    QStringList Com_argv_str;

   //----------------------------------------------------------

    char source[ 400 ];
    char search[ 100 ];
    memset( source , NULL , 400   );
    sprintf( source , "%s" ,    ui->lineEdit->text().toStdString().c_str()  );

    //file_name = "/home/justice/justice-script/testcase/gui_cmd.js";
    file_name = "/home/justice/justice-script/command/gui_cmd.js";

    long  para1 , para2 , para3;
    char *loc;

  while( 1 )
   {


     //------------------------------------------------------------

    sprintf( search , "%s" , "idfy" );
    loc = NULL;
    loc = strstr(  source ,search  );

    if( loc == NULL )
      {
         sprintf( search  , "%s" , "identify" );
         loc = strstr(  source ,search  );
      }

    if( loc != NULL )
      {

         if( strlen( search ) == 4 )
           P_parameter( loc , 4 , 2  ); // Search 4  char , 2 parameter

         if( strlen( search ) == 8 )
           P_parameter( loc , 8 , 2  ); // search 8 char , 2 parameter

           para1 = atoi(  ArgV_P1 );
           para2 = atoi( ArgV_P2 );


           if( ( para1 != 0 ) && ( para1 != 1 ) )
             {

               Listinfo->addItem( "Command parameter error");
               break;
             }


           Com_argv_str<<"idfy"<<  ArgV_P1<< ArgV_P2;
           Justice_Command_input( file_name , Com_argv_str );

           break;
      }


   //---------------------------------------------------------------------

    sprintf( search , "%s" , "gfea" );
    loc = NULL;
    loc = strstr(  source ,search  );

    if( loc == NULL )
       {
         // sprintf( search , "%s" , "Gfea" );
         // loc = strstr(  source ,search  );
       }

    if( loc != NULL )
      {

         if( strlen( search ) == 4 )
           P_parameter( loc , 4 , 2  ); // Search 4  char , 2 parameter

         // fix 0801
         para1 = atoi(  ArgV_P1 );
         para2 = atoi( ArgV_P2 );

         Com_argv_str<<"gfea"<<  ArgV_P1;
         Justice_Command_input( file_name , Com_argv_str );

         break;

     }

    //-----------------------------------------------------------------------

     sprintf( search , "%s" , "glog" );
     loc = NULL;
     loc = strstr(  source ,search  );

     if( loc == NULL )
        {
           //sprintf( search , "%s" , "Glog" );
           //loc = strstr(  source ,search  );
        }

     if( loc != NULL )
       {

          if( strlen( search ) == 4 )
            P_parameter( loc , 4 , 2  ); // Search 4  char , 2 parameter

            para1 = atoi( ArgV_P1 );
            para2 = atoi( ArgV_P2 );

            Com_argv_str<<"glog"<<  ArgV_P1 << ArgV_P2;
            Justice_Command_input( file_name , Com_argv_str );

            break;
       }

     //-------------------------------------------------------------------------

      sprintf( search , "%s" , "sirq" );
      loc = NULL;
      loc = strstr(  source ,search  );

      if( loc == NULL )
         {
            //sprintf( search , "%s" , "Sirq" );
            //loc = strstr(  source ,search  );
         }

      if( loc != NULL )
        {

           if( strlen( search ) == 4 )
             P_parameter( loc , 4 , 2  ); // Search 4  char , 2 parameter

             para1 = atoi( ArgV_P1 );
             para2 = atoi( ArgV_P2 );

             Com_argv_str<<"sirq"<<  ArgV_P1 << ArgV_P2;
             Justice_Command_input( file_name , Com_argv_str );

             break;
        }

    //-----------------------------------------------------------------------------

       sprintf( search , "%s" , "girq" );
       loc = NULL;
       loc = strstr(  source ,search  );

       if( loc == NULL )
          {
             //sprintf( search , "%s" , "Girq" );
             //loc = strstr(  source ,search  );
          }

       if( loc != NULL )
         {

            if( strlen( search ) == 4 )
              P_parameter( loc , 4 , 2  ); // Search 4  char , 2 parameter

              para1 = atoi(  ArgV_P1 );
              para2 = atoi( ArgV_P2 );

              Com_argv_str<<"girq"<<  ArgV_P1 << ArgV_P2;
              Justice_Command_input( file_name , Com_argv_str );

              break;
         }

    //----------------------------------------------------------------------

    sprintf( search , "%s" , "write" );
    loc = NULL;
    loc = strstr(  source ,search  );
    if( loc == NULL )
       {
          //sprintf( search , "%s" , "Write" );
          //loc = strstr(  source ,search  );
       }

    if( loc != NULL )
      {

        if( strlen( search ) == 5 )
           P_parameter( loc , 5 , 3  ); // Search 5  char , 3 parameter

           para1 = atoi( ArgV_P1 );
           para2 = atoi( ArgV_P2 );
           para3 = atoi( ArgV_P3 );

           if( para2 > 255 )
             {
               //Listinfo->clear();
               Listinfo->addItem( "Command parameter error");
               break;
             }


           Com_argv_str<<"write"<<  ArgV_P1 << ArgV_P2 << ArgV_P3;
           Justice_Command_input( file_name , Com_argv_str );

           break;
      }

    //--------------------------------------------------------------------------

     sprintf( search , "%s" , "rread" );
     loc = NULL;
     loc = strstr(  source ,search  );
     if( loc == NULL )
        {
           //sprintf( search , "%s" , "Rread" );
           //loc = strstr(  source ,search  );
        }

     if( loc != NULL )
       {

          if( strlen( search ) == 5 )
            P_parameter( loc , 5 , 2  ); // Search 5  char , 2 parameter

            para1 = atoi(  ArgV_P1 );
            para2 = atoi(  ArgV_P2 );

            Com_argv_str<<"rread" <<  ArgV_P1 << ArgV_P2;
            Justice_Command_input( file_name , Com_argv_str );

            break;

       }

    //---------------------------------------------------------------------------

    sprintf( search , "%s" , "read" );
    loc = NULL;
    loc = strstr(  source ,search  );
    if( loc == NULL )
       {
          //sprintf( search , "%s" , "Read" );
          //loc = strstr(  source ,search  );
       }

    if( loc != NULL )
      {

        if( strlen( search ) == 4 )
           P_parameter( loc , 4 , 2  ); // Search 4  char , 2 parameter

         para1 = atoi( ArgV_P1 );
         para2 = atoi( ArgV_P2 );

           if( para2 > 255 )
             {
               //Listinfo->clear();
               Listinfo->addItem( "Command parameter error");
               break;
             }

           Com_argv_str<<"read"<< ArgV_P1 << ArgV_P2;
           Justice_Command_input( file_name , Com_argv_str );

           break;
      }

    //------------------------------------------------------------------------------

    sprintf( search , "%s" , "gpci" );
    loc = NULL;
    loc = strstr(  source ,search  );
    if( loc == NULL )
       {
          //sprintf( search , "%s" , "Gpci" );
          //loc = strstr(  source ,search  );
       }
    if( loc != NULL )
      {

        if( strlen( search ) == 4 )
           P_parameter( loc , 4 , 2  ); // Search 4  char , 2 parameter

         para1 = atoi( ArgV_P1 );
         para2 = atoi( ArgV_P2 );

           Com_argv_str<<"gpci"<<  ArgV_P1 << ArgV_P2;
           Justice_Command_input( file_name , Com_argv_str );

           break;
      }

    //----------------------------------------------------------------------------------

    sprintf( search , "%s" , "gctrl" );
    loc = NULL;
    loc = strstr(  source ,search  );
    if( loc == NULL )
       {
          //sprintf( search , "%s" , "Gpci" );
          //loc = strstr(  source ,search  );
       }
    if( loc != NULL )
      {

        if( strlen( search ) == 4 )
           P_parameter( loc , 4 , 2  ); // Search 4  char , 2 parameter

         para1 = atoi(  ArgV_P1 );
         para2 = atoi( ArgV_P2 );

           Com_argv_str<<"gctrl"<<  ArgV_P1 << ArgV_P2;
           Justice_Command_input( file_name , Com_argv_str );

           break;
      }


     //---------------------------------------------------------------------------------

     sprintf( search , "%s" , "cfgpmcap" );
     loc = NULL;
     loc = strstr(  source ,search  );
     if( loc == NULL )
       {
           //sprintf( search , "%s" , "Cfgpmcap" );
           //loc = strstr(  source ,search  );
       }

     if( loc != NULL )
       {

          if( strlen( search ) == 8 )
            P_parameter( loc , 8 , 2  ); // search 8 char , 2 parameter

            para1 = atoi(  ArgV_P1 );
            para2 = atoi( ArgV_P2 );

            Com_argv_str<<"cfgpmcap"<<  ArgV_P1 << ArgV_P2;
            Justice_Command_input( file_name , Com_argv_str );

            break;
       }

     //-----------------------------------------------------------------------------------

     sprintf( search , "%s" , "cfgmsicap" );
     loc = NULL;
     loc = strstr(  source ,search  );
     if( loc == NULL )
       {
          // sprintf( search , "%s" , "Cfgmsicap" );
          // loc = strstr(  source ,search  );

     }

     if( loc != NULL )
       {

         if( strlen( search ) == 8 )
            P_parameter( loc , 8 , 2  ); // search 8 char , 2 parameter

            para1 = atoi(  ArgV_P1 );
            para2 = atoi( ArgV_P2 );

            Com_argv_str<<"cfgmsicap"<<  ArgV_P1 << ArgV_P2;
            Justice_Command_input( file_name , Com_argv_str );

            break;
        }

     //--------------------------------------------------------------------------------------

    sprintf( search , "%s" , "cfgpxcap" );
    loc = NULL;
    loc = strstr(  source ,search  );
    if( loc == NULL )
       {
           //sprintf( search , "%s" , "Cfgpxcap" );
           //loc = strstr(  source ,search  );
       }

     if( loc != NULL )
       {

          if( strlen( search ) == 8 )
            P_parameter( loc , 8 , 2  ); // search 8 char , 2 parameter

            para1 = atoi( ArgV_P1 );
            para2 = atoi( ArgV_P2 );

            Com_argv_str<<"cfgpxcap"<<  ArgV_P1 << ArgV_P2;
            Justice_Command_input( file_name , Com_argv_str );

            break;
       }

     //-------------------------------------------------------------------------------------

     sprintf( search , "%s" , "cfgmsixcap" );
     loc = NULL;
     loc = strstr(  source ,search  );
     if( loc == NULL )
       {
           //sprintf( search , "%s" , "Cfgmsixcap" );
           //loc = strstr(  source ,search  );
       }

     if( loc != NULL )
       {

          if( strlen( search ) == 10 )
            P_parameter( loc , 10  , 2  ); // search 10 char , 2 parameter

            para1 = atoi(  ArgV_P1 );
            para2 = atoi( ArgV_P2 );

            Com_argv_str<<"cfgmsixcap"<<  ArgV_P1<< ArgV_P2;
            Justice_Command_input( file_name , Com_argv_str );

            break;
       }

     //-------------------------------------------------------------------------------------

     sprintf( search , "%s" , "cls" );
     loc = NULL;
     loc = strstr(  source ,search  );

     if( loc != NULL )
       {

          Listinfo->clear();
          break;

       }


     //-----------------------------------------------------------------------------------------

     //sprintf( MS , "%s" , source );
     //QMessageBox::information( this,   ""  ,   MS  , QMessageBox::Ok);

     if( loc == NULL )
       {
           //Listinfo->clear();
           Listinfo->addItem( "Command is undefined");
           break;
       }


  } // ~ while( 1 )

  //----------------------------------------------------------------------------------------------

   TextEdit_display_code->setStyleSheet( "mone" );
   Listinfo->setStyleSheet( "mone" );
   Listinfo_W_pattern->setStyleSheet( "mone" );
   Listinfo_R_pattern->setStyleSheet( "mone" );

   m_control_scroll = false;

   ui->lineEdit->activateWindow();

   return;
}

\
void PCIE::on_lineEdit_editingFinished()
{


}

void PCIE::on_lineEdit_textEdited(const QString &arg1)
{

}


void PCIE::wheelEvent(QWheelEvent* event)
{


}

void PCIE::time_elapsed(  void  )
{




     char MS_Elapsed[ 100 ];

     if( ( str_length < 5 ) &&  ( time_diff > 2 ) )
       {
           //TEST_STOP = 1;
           //time_diff = 0;

       }

     time(&time_end);

     time_diff = time_end - time_start;    
     sec = ( time_diff % 60 ) - 1 ;
     minutes = time_diff / 60;
     hours = minutes / 60;
     minutes = minutes % 60;
     days = hours / 24;
     hours = hours % 24;

     if( sec < 0 )
       sec = 0;

     sprintf( MS_Elapsed , "Elapsed : Day %02d  : Hour %02d  : Min %02d  : Sec %02d", days, hours, minutes, sec);
     ui->label_13->setText( MS_Elapsed );

   if( timer_elapsed_enable == 0 )
     {

        stat_status_1->setText( "    Status : Stop" );

        timer_elapsed->stop();
        time_resert = false ;

        QString elapsed_time_log;
        elapsed_time_log = ui->label_13->text();

        if( m_save_log_checked == true )
          {

               sprintf( MS , "%s" ,  elapsed_time_log.toStdString().c_str()  );
               Record_log(  MS );

          }

        ui->Run1->setEnabled( true );
        ui->Stop1->setEnabled( false );

        //Run2->setEnabled( true );
        //Stop2->setEnabled( false );

        TextEdit_display_code->setStyleSheet( "mone" );
        Listinfo->setStyleSheet( "mone" );
        Listinfo_W_pattern->setStyleSheet( "mone" );
        Listinfo_R_pattern->setStyleSheet( "mone" );

    }


   // 2015 0628 node.js error
    if( TEST_STOP == 1 )
      {

        stat_status_1->setText( "    Status : Stop" );

        timer_elapsed->stop();
        time_resert = false ;

         QString elapsed_time_log;
         elapsed_time_log = ui->label_13->text();

         if( m_save_log_checked == true )
           {
             sprintf( MS , "%s" ,  elapsed_time_log.toStdString().c_str()  );
             Record_log(  MS );
           }

         ui->Run1->setEnabled( true );
         ui->Stop1->setEnabled( false );
         ui->Command_button->setEnabled( true );
         //Run2->setEnabled( true );
         //Stop2->setEnabled( false );



         TextEdit_display_code->setStyleSheet( 0 );
         Listinfo->setStyleSheet( 0 );
         Listinfo_W_pattern->setStyleSheet( 0 );
         Listinfo_R_pattern->setStyleSheet( 0 );

         Listinfo->scrollToBottom();

         if( file_name == "ID.js" )
          Get_Driver_version();

     }

     if( ( time_diff == 1 ) && ( m_argc == 2 ) )
       {
         //sleep( 1 );
          Auto_argv_run();
          m_argc = 1;
       }


}



void PCIE :: Start_Record_log( void )
{

    char t_str[ 10 ];
    time_t t_now;
    struct tm *tm_now;
    time( &t_now );
    tm_now= localtime( &t_now );
    strftime( t_str , 80 , "%Y%m%d%H%M%S", tm_now);

    char temp[ 50 ];
    char temp_log[ 50 ];

    if( m_modify_log_self_checked == false )
      sprintf( log_str , "PID:%x_%s_%s.Jlog" , process_ID , script_name   , t_str  );
    else
      sprintf( log_str , "%s.Jlog" , log_name_self.toStdString().c_str() );

    if( strlen( log_str ) > 50 )
      {

        log_str[ 46 ] = '.';
        log_str[ 47 ] = 'J';
        log_str[ 48 ] = 'l';
        log_str[ 49 ] = 'o';
        log_str[ 50 ] = 'g';
        log_str[ 51 ] = '\0';

      }

    if( strlen( log_str ) > 0 )
     {
       sprintf( MS , "chmod 777  %s" , log_str );
       system( MS );
     }

}



void PCIE :: Start_Input_log( void )
{

    sprintf(  Input_log_str  , "%s" , "Running.Jlog"  );

}


void PCIE ::Record_log( char *str )
{

   Log_file=fopen( log_str , "a" );

     if(  Log_file == 0 )
       return;
     else
       fprintf(   Log_file, "%s\n" , str  );

   fclose(  Log_file  );

}



void PCIE ::Input_log( char *str )
{

   Input_log_file =fopen( Input_log_str , "a" );

     if(  Input_log_file == 0 )
       return;
     else
       fprintf(  Input_log_file  , "%s\n" , str  );

   fclose(  Input_log_file  );

}


void PCIE::on_pushButton_21_clicked()
{

    QFont font( "Courier New", 11 , QFont::Normal ) ;
    Listinfo->setFont( font );


    Listinfo_W_pattern->setWindowFlags( Qt::WindowStaysOnTopHint );
    Listinfo_W_pattern->show();

    //Listinfo_W_pattern->setGeometry( 925  ,( 180 ) , ( 380 - 15 ) , 300 );
    Listinfo_W_pattern->setGeometry( ( 900 - 28 ) , 200 , ( 380 + 14 ) , 300 );


    Listinfo_W_pattern->show();

}


void PCIE::on_pushButton_22_clicked()
{

    QFont font( "Courier New", 11 , QFont::Normal ) ;
    Listinfo->setFont( font );

    Listinfo_R_pattern->setWindowFlags( Qt::WindowStaysOnTopHint );
    Listinfo_R_pattern->show();

    //Listinfo_R_pattern->setGeometry( 925 , ( 480 )  , ( 380 - 15 ) , 300 );
    Listinfo_R_pattern->setGeometry( ( 900 - 28 ) , 500 + 30 , ( 380 + 14 ) , 300 );

    Listinfo_R_pattern->show();

}


void PCIE:: loadSettings()
{

   QSettings * settings = 0;

   if( QFile( "Justice.ini" ).exists() )
     {

         settings = new QSettings( "Justice.ini", QSettings::IniFormat );

         settings->beginGroup("Setting");

         Product_tilte = settings->value(  "Product_tilte" , 1000  ).toString();

         Set_DUT_PCIE_number = settings->value(  "Set_DUT_PCIE_number" ,  1000  ).toString();

         Script_default_path = settings->value(  "Script_default_path" , 1000  ).toString();

         MAX_line_clear_form  = settings->value(  "MAX_line_clear_form" ,  1000  ).toString();

         Set_default_buffer_length = settings->value(  "Set_default_buffer_length" ,  1000  ).toString();

         settings->endGroup();

    }
  else
     QMessageBox::information(this,   "Justice"  ,   "                 ini file not exist                          "   , QMessageBox::Ok);

}


void LOG( QString str )
 {

     char WS[200 ];
     int j;

     sprintf( WS , "%s" , str.toStdString().c_str() );

     Save_log( WS ); // REcord log

 }


 void Parse_Msg( QString str )
 {

   char WS[200 ];
   int j;

   sprintf( WS , "%s" , str.toStdString().c_str() );

   // ---------------------- Justice status bar --------------------------

   if( file_name == "ID.js" )
   {

        sprintf( sch , "%s" , "Model" );
        lc = NULL;
        lc = strstr( WS  , sch  );

       if( lc != NULL )
         {

            j = 2; // shift 2 char

           char QS[100 ];

            QS[ 0 ] = ' ';
            QS[ 1 ] = ' ';

            for( int i = 4  ; i < 40 ; i++ )
             {
                QS [ j ] = WS [ i ];
                j++;
             }

            stat_model->setText( QS );

         }

        sprintf( sch , "%s" , "Firm" );
        lc = NULL;
        lc = strstr( WS  , sch  );

       if( lc != NULL )
         {

           j = 2 ; // shift 2 char

           char QS[100 ];

           QS[ 0 ] = ' ';
           QS[ 1 ] = ' ';

           for( int i = 4  ; i < 40 ; i++ )
           {
             QS [ j ] = WS [ i ];
             j++;
           }

           stat_FW_var->setText( QS );

         }

        sprintf( sch , "%s" , "MAXLBA" );
        lc = NULL;
        lc = strstr( WS  , sch  );

       if( lc != NULL )
         {

           j = 2; // shift 2 char

           char QS[100 ];

           QS[ 0 ] = ' ';
           QS[ 1 ] = ' ';

           for( int i = 4  ; i < 40 ; i++ )
            {

               QS [ j ] = WS [ i ];
               j++;

            }

           stat_MAX_LBA->setText(  QS );

           char LS[ 100 ];
           sprintf( LS , "%c%c%c%c%c%c%c%c%c%c" , WS[ 9 + 4 ] , WS[  10 + 4  ] , WS[ 11 + 4 ] , WS[  12 + 4  ] , WS[ 13 + 4 ] , WS[  14 + 4 ] , WS[ 15 + 4 ] , WS[  16 + 4  ]  , WS[ 17 + 4 ] , WS[  18 + 4  ] );

           MAX_LBA = atoi( LS );

           //DUT_capacity = ( (  MAX_LBA * 512 )  / 1024  / 1024 / 1024 );
           DUT_capacity = ( (  MAX_LBA * 512 )  / 1000  / 1000 / 1000 );  // 0728 Lili

           sprintf( QS , "  Capacity : %d GB"  , DUT_capacity ) ;
           stat_capacity->setText( QS );

          }


    }

   // ----------------------------------- get driver version-------------------------------

  //if( file_name == "/home/justice/justice-script/testcase/driver.js" )
   if( file_name == "/home/justice/justice-script/command/driver.js" )
   {

        sprintf( sch , "%s" , "Driver Version:" );
        lc = NULL;
        lc = strstr( WS  , sch  );
        char QS[100 ];

        int j = 0;

       if( lc != NULL )
         {


           for( int i = 15  ; i < 40 ; i++ )
            {

               Jdriver_ver [ j ] = WS [ i ];
               j++;

            }

           sprintf( QS , " Driver ver : %s"  , Jdriver_ver );
           stat_driver_ver->setText( QS );

         }

    }

   // ---------------------- ------------------------------------- --------------------------

     sprintf( sch , "%s" , "CLS" );
     lc = NULL;
     lc = strstr( WS  , sch  );

     if( lc != NULL )
      Listinfo->clear();

   // ---------------------- ------------------------------------- --------------------------

     sprintf( sch , "%s" , "data miscompare" );
     lc = NULL;
     lc = strstr( WS  , sch  );

     if( lc != NULL )
       {
         TEST_STOP = 1;
         Listinfo->scrollToBottom();
       }

  // ---------------------- ------------------------------------- --------------------------
     sprintf( sch , "%s" , "end" );
     lc = NULL;
     lc = strstr( WS  , sch  );

     if( lc != NULL )
       {
         //TEST_STOP = 1;
         Listinfo->scrollToBottom();
       }

   // ---------------------- ------------------------------------- --------------------------

        sprintf( sch , "%s" , "error" );
        lc = NULL;
        lc = strstr( WS  , sch  );

        if( lc != NULL )
          {
            TEST_STOP = 1;
            //Listinfo->scrollToBottom();
          }

   // ---------------------- ------------------------------------- --------------------------


        sprintf( sch , "%s" , "fail" );
        lc = NULL;
        lc = strstr( WS  , sch  );

        if( lc != NULL )
        {

          TEST_STOP = 1;

        }

    // ---------------------- ------------------------------------- --------------------------


 }

void Save_log(  char * str  )
 {

   if( log_mode_sw == 0 )   // commput input
     {

         FILE *Input_log_file;

         Input_log_file =fopen( Input_log_str , "a" );

         if(  Input_log_file == 0 )
          return;
         else
          fprintf(  Input_log_file  , "%s\n" , str  );

         fclose(  Input_log_file  );

     }
   else
     {


         FILE *Log_file ;

         Log_file = fopen( log_str , "a" );

         if(  Log_file == 0 )
            return  ;

          else
             fprintf(   Log_file, "%s\n" , str  );

          fclose(  Log_file  );

      }


 }


void PCIE::on_Run1_clicked()
{

    QFont font( "Courier New", 11 , QFont::Normal ) ;
    Listinfo->setFont( font );

    Command_test_enable = 0;


    if( ( fileName_node == NULL ) || ( strlen( fileName_node.toStdString().c_str() ) == 0 )  )
      {

          QMessageBox::information( this, tr("Justice"), tr(  "Please select JS file          ")  );
          return;
      }

    Listinfo_W_pattern->setWindowFlags( 0 );
    Listinfo_R_pattern->setWindowFlags( 0 );
    TextEdit_display_code->setWindowFlags( 0 );

    this->setWindowFlags( Qt::WindowStaysOnBottomHint );
    this->show();

    Listinfo -> setParent( NULL );

    Listinfo->show();
    TextEdit_display_code->show();
    Listinfo_W_pattern->show();
    Listinfo_R_pattern->show();
    Listinfo->setGeometry( 100 , 200 ,  500  , 515 );
    TextEdit_display_code->setGeometry(  570 , 200 ,  500   , 515 );

    Listinfo_W_pattern->setGeometry( ( 900 - 28 ) , 200 , ( 380 + 14 ) , 300 );
    Listinfo_R_pattern->setGeometry( ( 900 - 28 ) , 500 + 30 , ( 380 + 14 ) , 300 );

    TextEdit_display_code->setStyleSheet( "border: 1px solid orange" );
    Listinfo->setStyleSheet(  "border: 1px solid orange");
    Listinfo_W_pattern->setStyleSheet(   "border: 1px solid orange");
    Listinfo_R_pattern->setStyleSheet(  "border: 1px solid orange" );

    command_input_mode = false;

    log_mode_sw = 1;
    m_control_scroll = true;


    if( m_save_log_checked == true )
     Start_Record_log();

    M.m_msg_list1.clear();

   if( m_argc == 1 )    // script_argv_str : process.argv[3]  0  nvme0 , 1 nvme 1 , 2 nvme 2 , 3 nvme 3
     M.cmd1->execute(QString( fileName_node  ), script_argv_str  );

   if( m_argc == 2 )
     M.cmd1->execute(QString( filePath_args ), QStringList()   );

   TEST_STOP = 0;

   timer_elapsed->stop();

   time_resert = false ;

   timer_elapsed_enable = 1;

   stat_status_1->setText( "    Status : Run" );

   if( time_resert == false )
     {
        time(&time_start);
        time_resert = true ;
     }

   days = 0;
   hours = 0;
   minutes = 0;
   sec = 0;

   timer_elapsed->stop();
   timer_elapsed->start( 1000 );   // 1 second

   mode_sw = 1;

   ui->Run1->setEnabled( false );
   ui->Stop1->setEnabled( true );

   //Run2->setEnabled( false );
   //Stop2->setEnabled( true );

   ui->Command_button->setEnabled( false );

   // if message less 5 chara and elapsed time more than 5 second , system stop and elpased time stop
   str_length = 0;
   str_length_temp = 0;

}



void PCIE::on_Stop1_clicked()
{

   M.cmd1->cancel();
   M.cmd1->freeable();

   ui->Run1->setEnabled( true );
   ui->Stop1->setEnabled( false );

   //Run2->setEnabled( true );
   //Stop2->setEnabled( false );

   ui->Command_button->setEnabled( true );


}



void PCIE::on_Run1_pressed()
{

}


void PCIE::on_Run1_released()
{

}


void PCIE::on_Run1_clicked(bool checked)
{

}


void PCIE::on_Clear_clicked()
{

    QFont font( "Courier New", 11 , QFont::Normal ) ;
    Listinfo->setFont( font );

    Listinfo->clear();

    M.m_cnt1 = 0;
    M.m_cnt1_temp = 0;
    M.m_msg_list1.clear();

}

void PCIE::on_Clear_pressed()
{


}

void PCIE::on_Idfy1_Button_clicked()
{

    Command_test_enable = 0;

    Listinfo_W_pattern->close();
    Listinfo_R_pattern->close();
    TextEdit_display_code->close();


    QFont font( "Courier New", 11 , QFont::Normal ) ;
    Listinfo->setFont( font );

    M.m_cnt1 = 0;
    M.m_cnt1_temp = 0;
    M.m_msg_list1.clear();

    Listinfo -> setParent( this );
    Listinfo->setGeometry( ( 45 - 4 )   ,  100 ,  1079   , 570 );
    Listinfo->show();

    log_mode_sw = 0;
    command_input_mode = true;
    m_control_scroll = false;

    M.m_cnt1 = 0;
    M.m_cnt1_temp = 0;
    M.m_msg_list1.clear();

    QStringList Com_argv_str;

    //file_name = "/home/justice/justice-script/testcase/gui_cmd.js";
    file_name = "/home/justice/justice-script/command/gui_cmd.js";

    Com_argv_str<<"idfy"<<"1";
    Justice_Command_input( file_name , Com_argv_str );

    Listinfo->setStyleSheet(  "none");
}

void PCIE::on_Idfy0_Button_pressed()
{

}

void PCIE::on_Idfy0_Button_clicked()
{
    Command_test_enable = 0;

    Listinfo_W_pattern->close();
    Listinfo_R_pattern->close();
    TextEdit_display_code->close();

    QFont font( "Courier New", 11 , QFont::Normal ) ;
    Listinfo->setFont( font );

    M.m_cnt1 = 0;
    M.m_cnt1_temp = 0;
    M.m_msg_list1.clear();

    Listinfo-> setParent( this );
    Listinfo->setGeometry( ( 45 - 4 ) ,  100 ,  1079   , 570 );
    Listinfo->show();

    log_mode_sw = 0;
    command_input_mode = true;
    m_control_scroll = false;

    M.m_cnt1 = 0;
    M.m_cnt1_temp = 0;
    M.m_msg_list1.clear();

    QStringList Com_argv_str;

    //file_name = "/home/justice/justice-script/testcase/gui_cmd.js";
    file_name = "/home/justice/justice-script/command/gui_cmd.js";

    Com_argv_str<<"idfy"<<"0";
    Justice_Command_input( file_name , Com_argv_str );

    Listinfo->setStyleSheet(  "none");
}

void PCIE::on_PCI_Button_clicked()
{

    Command_test_enable = 0;

    Listinfo_W_pattern->close();
    Listinfo_R_pattern->close();
    TextEdit_display_code->close();


    M.m_cnt1 = 0;
    M.m_cnt1_temp = 0;
    M.m_msg_list1.clear();


    Listinfo -> setParent( this );
    Listinfo->setGeometry( ( 45 - 4 )  ,  100 ,  1079   , 570 );

    QFont font( "Courier New", 11 , QFont::Normal ) ;
    Listinfo->setFont( font );

    Listinfo->show();

    //QFont font1( "Ubuntu", 10 , QFont::Normal ) ;
    //Listinfo->setFont( font1 );


    log_mode_sw = 0;
    command_input_mode = true;
    m_control_scroll = false;

    M.m_cnt1 = 0;
    M.m_cnt1_temp = 0;
    M.m_msg_list1.clear();

    QStringList Com_argv_str;

    //file_name = "/home/justice/justice-script/testcase/gui_cmd.js";
    file_name = "/home/justice/justice-script/command/gui_cmd.js";

    Com_argv_str<<"gpci"<<"0";
    Justice_Command_input( file_name , Com_argv_str );

    Listinfo->setStyleSheet( "none" );





}



void PCIE::on_CTRL_Button_clicked()
{

    Command_test_enable = 0;

    Listinfo_W_pattern->close();
    Listinfo_R_pattern->close();
    TextEdit_display_code->close();



    QFont font( "Courier New", 11 , QFont::Normal ) ;
    Listinfo->setFont( font );

    M.m_cnt1 = 0;
    M.m_cnt1_temp = 0;
    M.m_msg_list1.clear();

    Listinfo -> setParent( this );
    Listinfo->setGeometry( ( 45 - 4 )  ,  100 ,  1079   , 570 );
    Listinfo->show();

    log_mode_sw = 0;
    command_input_mode = true;
    m_control_scroll = false;

    M.m_cnt1 = 0;
    M.m_cnt1_temp = 0;
    M.m_msg_list1.clear();

    QStringList Com_argv_str;

   // file_name = "/home/justice/justice-script/testcase/gui_cmd.js";
   file_name = "/home/justice/justice-script/command/gui_cmd.js";

    Com_argv_str<<"gctrl"<<"0";
    Justice_Command_input( file_name , Com_argv_str );

    Listinfo->setStyleSheet("none");
}

void PCIE::on_Histoty_button_clicked()
{

    QFont font( "Courier New", 11 , QFont::Normal ) ;
    Listinfo->setFont( font );

    T_com.setWindowFlags( Qt::WindowStaysOnTopHint );
    T_com.move( QApplication::desktop()->screen()->rect().center() - T_com.rect().center());
    T_com.LOadHistory();
    T_com.show();


}


void PCIE::on_Command_button_clicked()
{

   ui->Run1->setEnabled( false );
   ui->Stop1->setEnabled( true );
   //Run2->setEnabled( false );
   //Stop2->setEnabled( true );


   days_c = 0;
   hours_c = 0;
   minutes_c = 0;
   sec_c = 0;
   //time_diff_c = 0;

   char MS_Elapsed_c[ 100 ];
   sprintf( MS_Elapsed_c , "Elapsed : Day %02d  : Hour %02d  : Min %02d  : Sec %02d", days_c, hours_c, minutes_c, sec_c);
   Elapsed_c_label = new QLabel( MS_Elapsed_c );
   Elapsed_c_label->setGeometry( 30, 320 , 431 , 20 );
   Elapsed_c_label->setText( MS_Elapsed_c );
   //Elapsed_c_label->show();

   Listinfo->setParent( this );

   loadSettings_KeyIn();

    K_test.setWindowFlags( Qt::WindowStaysOnTopHint );
    K_test.move( QApplication::desktop()->screen()->rect().center() -  K_test.rect().center());

    K_test.show();

}


void loadSettings_KeyIn()
{

   QSettings * settings = 0;

   if( QFile( "Command.ini" ).exists() )
     {

         settings = new QSettings( "Command.ini", QSettings::IniFormat );

         settings->beginGroup("Setting");

         K_Test_count = settings->value(  "Test_count" ,  1000  ).toString();
         K_Admin = settings->value(  "Admin" ,  1000  ).toString();
         K_OPcode = settings->value(  "OPcode" ,  1000  ).toString();
         K_DW1 = settings->value(  "DW1" ,  1000  ).toString();
         K_DW10 = settings->value(  "DW10" ,  1000  ).toString();
         K_DW11 = settings->value(  "DW11" ,  1000  ).toString();
         K_DW12 = settings->value(  "DW12" ,  1000  ).toString();

         settings->endGroup();

    }
  else
     QMessageBox::information( NULL,   "Justice"  ,   "                 Command.ini file not exist                          "   , QMessageBox::Ok);

}



void SaveSettings_KeyIn()
{

    QString com_buf[ 100 ];
    unsigned char context[64]={0};
    memset(context,0,64);
    ulong * temp = (ulong *)context;

    *temp = strtoul(K_OPcode.toLatin1(),NULL,16);
      temp = (ulong *)&context[4];;
    *temp = strtoul(K_DW1.toLatin1(),NULL,16);
    temp = (ulong *)&context[40];
    *temp = strtoul(K_DW10.toLatin1(),NULL,16);
     temp = (ulong *)&context[44];
    *temp = strtoul(K_DW11.toLatin1(),NULL,16);
     temp = (ulong *)&context[48];
    *temp = strtoul(K_DW12.toLatin1(),NULL,16);

    QSettings * settings = 0;

    if( QFile( "Command.ini" ).exists() )
      {

          settings = new QSettings( "Command.ini", QSettings::IniFormat );

          settings->beginGroup("Setting");

          settings->setValue("Test_count", K_Test_count );
          settings->setValue("Admin", K_Admin );

          settings->setValue( "OPcode",  K_OPcode);


          settings->setValue("DW1",K_DW1  );
          settings->setValue("DW10", K_DW10 );
          settings->setValue("DW11", K_DW11 );
          settings->setValue("DW12", K_DW12 );

          settings->endGroup();

     }
   else
      QMessageBox::information( NULL,   "Justice"  ,   "                Command.ini file not exist                          "   , QMessageBox::Ok);

        
    FILE *Sck;
    Sck=fopen( "cmd.txt" ,"w");

  if( Sck==NULL )
    {

       QMessageBox::information( NULL,   "Justice"  ,  "          Write com.txt fail                     "   , QMessageBox:: Information);
       fclose(Sck);
    }
  else
    {
        /*
        fprintf(  Sck , "%s\n" ,  "00 00 00 00 00 00 00 00" );
        fprintf(  Sck , "%s\n" ,  "00 00 00 00 00 00 00 00" );
        fprintf(  Sck , "%s\n" ,  "00 00 00 00 00 00 00 00" );
        fprintf(  Sck , "%s\n" ,  "00 00 00 00 00 00 00 00" );
        fprintf(  Sck , "%s\n" ,  "00 00 00 00 00 00 00 00" );
        fprintf(  Sck , "%s\n" ,  "00 00 00 00 00 00 00 00" );
        fprintf(  Sck , "%s\n" ,  "00 00 00 00 00 00 00 00" );
        fprintf(  Sck , "%s\n" ,  "00 00 00 00 00 00 00 00" );
        */

       for( int byte_No = 0 ; byte_No < 100 ;byte_No++ )
        {
           sprintf( MS , "%02X" , 0x00 );
           com_buf[ byte_No ] = MS;
        }

       QString str;
      //sprintf(str,"%04x",K_OPcode.toInt());
       str = K_OPcode.left( 2 );
       com_buf[ 0 ] = str;

       str = K_DW1.left( 2 );     // DW 1
       com_buf[ 4 ] = str;
       str = K_DW1.mid( 2 ,2 );
       com_buf[ 5 ] = str;
       str = K_DW1.mid(  4, 2 );
       com_buf[ 6 ] = str;
       str = K_DW1.right(  2 );
       com_buf[ 7 ] = str;

       str = K_DW10.left( 2 );    // DW 10
       com_buf[ 40 ] = str;
       str = K_DW10.mid( 2 ,2 );
       com_buf[ 41 ] = str;
       str = K_DW10.mid(  4, 2 );
       com_buf[ 42 ] = str;
       str = K_DW10.right(  2 );
       com_buf[ 43 ] = str;


       str = K_DW11.left( 2 );    // DW 11
       com_buf[ 44 ] = str;
       str = K_DW11.mid( 2 ,2 );
       com_buf[ 45 ] = str;
       str = K_DW11.mid(  4, 2 );
       com_buf[ 46 ] = str;
       str = K_DW11.right(  2 );
       com_buf[ 47 ] = str;

       str = K_DW12.left( 2 );    // DW 12
       com_buf[ 48 ] = str;
       str = K_DW12.mid( 2 ,2 );
       com_buf[ 49 ] = str;
       str = K_DW12.mid(  4, 2 );
       com_buf[ 50 ] = str;
       str = K_DW12.right(  2 );
       com_buf[ 51 ] = str;




       for( int addr = 0 ; addr < 64 ; addr++ )
        {
           //fprintf(  Sck , "%02s " ,  com_buf[ addr ].toStdString().c_str() );
            fprintf(  Sck , "%02x " ,  context[ addr ] );
           if( ( ( addr + 1 ) % 8 ) == 0 )
             fprintf(  Sck , "\n" );

        }


      fclose(Sck);

    }

  

}

void PCIE::Run2_clicked( void )
{

    QFont font( "Courier New", 11 , QFont::Normal ) ;
    Listinfo->setFont( font );

    Command_test_enable = 0;


    if( ( fileName_node == NULL ) || ( strlen( fileName_node.toStdString().c_str() ) == 0 )  )
      {

          QMessageBox::information( this, tr("Justice"), tr(  "Please select JS file          ")  );
          return;
      }

    Listinfo_W_pattern->setWindowFlags( 0 );
    Listinfo_R_pattern->setWindowFlags( 0 );
    TextEdit_display_code->setWindowFlags( 0 );

    this->setWindowFlags( Qt::WindowStaysOnBottomHint );
    this->show();

    Listinfo -> setParent( NULL );

    Listinfo->show();
    TextEdit_display_code->show();
    Listinfo_W_pattern->show();
    Listinfo_R_pattern->show();
    Listinfo->setGeometry( 100 , 200 ,  500  , 515 );
    TextEdit_display_code->setGeometry(  570 , 200 ,  500   , 515 );

    Listinfo_W_pattern->setGeometry( ( 900 - 28 ) , 200 , ( 380 + 14 ) , 300 );
    Listinfo_R_pattern->setGeometry( ( 900 - 28 ) , 500 + 30 , ( 380 + 14 ) , 300 );

    TextEdit_display_code->setStyleSheet( "border: 1px solid orange" );
    Listinfo->setStyleSheet(  "border: 1px solid orange");
    Listinfo_W_pattern->setStyleSheet(   "border: 1px solid orange");
    Listinfo_R_pattern->setStyleSheet(  "border: 1px solid orange" );

    command_input_mode = false;

    log_mode_sw = 1;
    m_control_scroll = true;


    if( m_save_log_checked == true )
     Start_Record_log();

    M.m_msg_list1.clear();

   if( m_argc == 1 )    // script_argv_str : process.argv[3]  0  nvme0 , 1 nvme 1 , 2 nvme 2 , 3 nvme 3
     M.cmd1->execute(QString( fileName_node  ), script_argv_str  );

   if( m_argc == 2 )
     M.cmd1->execute(QString( filePath_args ), QStringList()   );

   TEST_STOP = 0;

   timer_elapsed->stop();

   time_resert = false ;

   timer_elapsed_enable = 1;

   stat_status_1->setText( "    Status : Run" );

   if( time_resert == false )
     {
        time(&time_start);
        time_resert = true ;
     }

   days = 0;
   hours = 0;
   minutes = 0;
   sec = 0;

   timer_elapsed->stop();
   timer_elapsed->start( 1000 );   // 1 second

   mode_sw = 1;



   QPushButton *Run2  = new QPushButton();
   QPushButton *Stop2 = new QPushButton();
   Run2->setParent( this );
   Stop2->setParent( this );
   Run2->setGeometry( 10 , 100 , 50 , 25  );
   Run2->setText( "Run");
   Stop2->setGeometry( 60 , 100 , 50 , 25 );
   Stop2->setText( "Stop");
   Run2->show();
   Stop2->show();
   Run2->setEnabled( 0 );
   Stop2->setEnabled( 1 );


   ui->Command_button->setEnabled( false );

   // if message less 5 chara and elapsed time more than 5 second , system stop and elpased time stop
   str_length = 0;
   str_length_temp = 0;




}

void PCIE::Stop2_clicked( void )
{

    M.cmd1->cancel();
    M.cmd1->freeable();

    QPushButton *Run2  = new QPushButton();
    QPushButton *Stop2 = new QPushButton();
    Run2->setParent( this );
    Stop2->setParent( this );
    Run2->setGeometry( 10 , 100 , 50 , 25  );
    Run2->setText( "Run");
    Stop2->setGeometry( 60 , 100 , 50 , 25 );
    Stop2->setText( "Stop");
    Run2->show();
    Stop2->show();
    Run2->setEnabled( 1 );
    Stop2->setEnabled( 0 );

    ui->Command_button->setEnabled( true );

}














