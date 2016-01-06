#include "keyin_test.h"
#include "ui_keyin_test.h"
#include "QMessageBox"
#include "QListWidget"
#include "QTextEdit"
#include <QDebug>
#include <QTimer>
#include <QObject>

// --------- time viable -------------
time_t time_start_c, time_end_c;
int  time_diff_c;
bool time_resert_c ;
long sec_c , minutes_c  , hours_c ,  days_c;

QTimer *timer_elapsed;
bool timer_elapsed_enable_c;
//------------------------------------

extern int mode_sw;
extern QListWidget  *Listinfo;
extern QTextEdit    *TextEdit_display_code;
extern QListWidget  *Listinfo_W_pattern;
extern QListWidget  *Listinfo_R_pattern;

extern void loadSettings_KeyIn();
extern void SaveSettings_KeyIn();

extern QString  K_Test_count;
extern QString  K_Admin;
extern QString  K_OPcode;
extern QString  K_DW1;
extern QString  K_DW10;
extern QString  K_DW11;
extern QString  K_DW12;
int hex_digit_value(char x);

QString OPCODE;
bool ASMIN;
QString TEST_COUNT;
QString PARA1;
QString PARA2;
QString PARA3;
QString PARA4;

extern QPushButton  *Run2;
extern QPushButton  *Stop2;

//int Input_string_product( void );

extern char ArgV_P1[ 100 ] , ArgV_P2[ 100 ] , ArgV_P3[ 100 ];   // command input temp buffer
extern QString  filePath , file_name;

char ArgV_P0[ 100 ];
extern bool Command_test_enable;

extern bool log_mode_sw;  // script mode : 1 , input mode : 0
extern bool command_input_mode;
extern bool m_save_log_checked;
extern bool m_control_scroll;

extern char admin_str[ 100 ];
extern char Count_str[ 100 ];

char buff[300] = "CMDBYTE=" ;

QLabel *Elapsed_c_label;

#define CMM_LEN 64
#define MAX_LINE 1024
extern  bool TEST_STOP;

KeyIn_test::KeyIn_test(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeyIn_test)
{
    ui->setupUi(this);

    ui->pushButton->setEnabled( true );
    ui->pushButton_2->setEnabled( false );


    loadSettings_KeyIn();

    ui->TestCount_Edit->setText( K_Test_count );


     if( K_Admin == "1" )
        ui->checkBox->setChecked( true );
     else
        ui->checkBox->setChecked( false );

    ui->OPCode->setText( K_OPcode );
    ui->DW1_Edit->setText( K_DW1);
    ui->DW10_Edit->setText( K_DW10 );
    ui->DW11_Edit->setText( K_DW11 );
    ui->DW12_Edit->setText( K_DW12);

   days_c = 0;
   hours_c = 0;
   minutes_c = 0;
   sec_c = 0;
   time_diff_c = 0;

   timer_elapsed_c = new QTimer(this);
   QObject::connect( timer_elapsed_c , SIGNAL(timeout()) , this , SLOT( time_elapsed_c() )  )   ;

   /*
   char MS_Elapsed_c[ 100 ];
   sprintf( MS_Elapsed_c , "Elapsed : Day %02d  : Hour %02d  : Min %02d  : Sec %02d", days_c, hours_c, minutes_c, sec_c);
   Elapsed_c_label = new QLabel( MS_Elapsed_c );
   Elapsed_c_label->setGeometry( 30, 320 , 431 , 20 );
   Elapsed_c_label->setText( MS_Elapsed_c );
   Elapsed_c_label->setParent( this );
   Elapsed_c_label->show();
   */


   char MS_Elapsed_c[ 100 ];
   sprintf( MS_Elapsed_c , "Elapsed : Day %02d  : Hour %02d  : Min %02d  : Sec %02d", days_c, hours_c, minutes_c, sec_c);
   ui->label_7->setText( MS_Elapsed_c );


}

KeyIn_test::~KeyIn_test()
{
    delete ui;
}

void KeyIn_test::on_Save_pushButton_clicked()
{


     K_Test_count= ui->TestCount_Edit->toPlainText();

     if( ui->checkBox->isChecked() == true )
        K_Admin = '1';
     else
        K_Admin = '0';

     K_OPcode= ui->OPCode->toPlainText();
     K_OPcode = K_OPcode.left( 2 );

     K_DW1 = ui->DW1_Edit->toPlainText();
     K_DW10 = ui->DW10_Edit->toPlainText();
     K_DW11 = ui->DW11_Edit->toPlainText();
     K_DW12 = ui->DW12_Edit->toPlainText();

    SaveSettings_KeyIn();



    QMessageBox::information( this,   "Justice"  ,  "          Save Setting OK                     "   , QMessageBox:: Information);



}

void KeyIn_test::on_Load_pushButton_clicked()
{


    loadSettings_KeyIn();

    ui->TestCount_Edit->setText( K_Test_count );


     if( K_Admin == "1" )
        ui->checkBox->setChecked( true );
      else
        ui->checkBox->setChecked( false);

    ui->OPCode->setText( K_OPcode );
    ui->DW1_Edit->setText( K_DW1);
    ui->DW10_Edit->setText( K_DW10 );
    ui->DW11_Edit->setText( K_DW11 );
    ui->DW12_Edit->setText( K_DW12);





}

void KeyIn_test::on_pushButton_2_clicked()
{


    Listinfo->setStyleSheet( 0 );
    Listinfo_W_pattern->setStyleSheet( 0 );
    Listinfo_R_pattern->setStyleSheet( 0 );


    ui->pushButton->setEnabled( true );
    ui->pushButton_2->setEnabled( false );

    M.cmd1->cancel();
    M.cmd1->freeable();

}

void KeyIn_test::on_pushButton_4_clicked()
{


    Listinfo->close();
    Listinfo_W_pattern->close();
    Listinfo_R_pattern->close();

    this->close();

    //Run2->setEnabled( true );
    //Stop2->setEnabled( false );
    //ui->Command_button->setEnabled( true );


}

void KeyIn_test::on_pushButton_3_clicked()
{
   ui->TestCount_Edit->clear();

    ui->OPCode->clear();
    ui->DW1_Edit->clear();
    ui->DW10_Edit->clear();
    ui->DW11_Edit->clear();
    ui->DW12_Edit->clear();

     ui->checkBox->setChecked( false );
}

void KeyIn_test::on_pushButton_clicked()
{


    Listinfo->setStyleSheet(  "border: 1px solid orange");
    Listinfo_W_pattern->setStyleSheet(   "border: 1px solid orange");
    Listinfo_R_pattern->setStyleSheet(  "border: 1px solid orange" );


    char C_Str[ 10 ];

    K_Test_count= ui->TestCount_Edit->toPlainText();

    if( ui->checkBox->isChecked() == true )
      {
        K_Admin = '1';
        ASMIN = true;
      }
    else
      {
        K_Admin = '0';
        ASMIN = false;

      }

    K_OPcode= ui->OPCode->toPlainText();
    K_OPcode = K_OPcode.left( 2 );

    K_DW1 = ui->DW1_Edit->toPlainText();
    K_DW10 = ui->DW10_Edit->toPlainText();
    K_DW11 = ui->DW11_Edit->toPlainText();
    K_DW12 = ui->DW12_Edit->toPlainText();


    //if( !Input_string_product() )
    //    return;


   if( strlen( K_Test_count.toStdString().c_str() ) == 0 )
     {

       Listinfo->setStyleSheet( 0 );
       Listinfo_W_pattern->setStyleSheet( 0 );
       Listinfo_R_pattern->setStyleSheet( 0 );

       QMessageBox::information( this ,   "Justice"  , "Please input test count          "  , QMessageBox::Warning);
       return;
     }


   if( strlen( K_OPcode.toStdString().c_str() ) != 2 )
     {

       Listinfo->setStyleSheet( 0 );
       Listinfo_W_pattern->setStyleSheet( 0 );
       Listinfo_R_pattern->setStyleSheet( 0 );

       QMessageBox::information(this ,   "Justice"  , "OP code length isn't 2 byte          "  , QMessageBox::Warning);
       return;
     }
  else
    {

       strcpy( C_Str , K_OPcode.toStdString().c_str() );

       for( int i = 0 ; i < 2 ;i++ )
         {

            if( !hex_digit_value( C_Str[i] ) )
             {

                Listinfo->setStyleSheet( 0 );
                Listinfo_W_pattern->setStyleSheet( 0 );
                Listinfo_R_pattern->setStyleSheet( 0 );

                QMessageBox::information( this ,   "Justice"  , "OP code valuw error , it isn't integer           "  , QMessageBox::Warning);
               return;

             }

         }

    }


  if( strlen( K_DW1.toStdString().c_str() ) != 8 )
    {

      Listinfo->setStyleSheet( 0 );
      Listinfo_W_pattern->setStyleSheet( 0 );
      Listinfo_R_pattern->setStyleSheet( 0 );

      QMessageBox::information( this,   "Justice"  , "DW1 length isn't' 8 byte          "  , QMessageBox::Warning);
      return;
    }
   else
    {


      strcpy( C_Str , K_DW1.toStdString().c_str() );

      for( int i = 0 ; i < 8 ;i++ )
        {

           if( !hex_digit_value( C_Str[i] ) )
            {

               Listinfo->setStyleSheet( 0 );
               Listinfo_W_pattern->setStyleSheet( 0 );
               Listinfo_R_pattern->setStyleSheet( 0 );

               QMessageBox::information( this ,   "Justice"  , "DW1 valuw error , it isn't integer           "  , QMessageBox::Warning);
              return;

            }

        }


      }


  if( strlen( K_DW10.toStdString().c_str() ) != 8 )
    {

      Listinfo->setStyleSheet( 0 );
      Listinfo_W_pattern->setStyleSheet( 0 );
      Listinfo_R_pattern->setStyleSheet( 0 );

      QMessageBox::information(  this ,   "Justice"  , "DW10 length isn't' 8 byte          "  , QMessageBox::Warning);
      return;
    }
  else
    {

       strcpy( C_Str , K_DW10.toStdString().c_str() );

       for( int i = 0 ; i < 8 ;i++ )
          {

              if( !hex_digit_value( C_Str[i] ) )
               {

                  Listinfo->setStyleSheet( 0 );
                  Listinfo_W_pattern->setStyleSheet( 0 );
                  Listinfo_R_pattern->setStyleSheet( 0 );

                  QMessageBox::information( this ,   "Justice"  , "DW10 valuw error , it isn't integer           "  , QMessageBox::Warning);
                 return;

               }

          }

    }



  if( strlen( K_DW11.toStdString().c_str() ) != 8 )
    {

      Listinfo->setStyleSheet( 0 );
      Listinfo_W_pattern->setStyleSheet( 0 );
      Listinfo_R_pattern->setStyleSheet( 0 );

      QMessageBox::information( this,   "Justice"  , "DW11 length isn't' 8 byte          "  , QMessageBox::Warning);
      return;
    }
  else
    {

       strcpy( C_Str , K_DW11.toStdString().c_str() );

       for( int i = 0 ; i < 8 ;i++ )
          {

              if( !hex_digit_value( C_Str[i] ) )
               {
                  Listinfo->setStyleSheet( 0 );
                  Listinfo_W_pattern->setStyleSheet( 0 );
                  Listinfo_R_pattern->setStyleSheet( 0 );

                  QMessageBox::information( this,   "Justice"  , "DW11 valuw error , it isn't integer           "  , QMessageBox::Warning);
                 return;

               }

          }

    }



  if( strlen( K_DW12.toStdString().c_str() ) != 8 )
    {
      Listinfo->setStyleSheet( 0 );
      Listinfo_W_pattern->setStyleSheet( 0 );
      Listinfo_R_pattern->setStyleSheet( 0 );

      QMessageBox::information( this,   "Justice"  , "DW12 length isn't ' 8 byte          "  , QMessageBox::Warning);
      return;
    }
  else
    {

       strcpy( C_Str , K_DW12.toStdString().c_str() );

       for( int i = 0 ; i < 8 ;i++ )
          {

              if( !hex_digit_value( C_Str[i] ) )
               {
                  Listinfo->setStyleSheet( 0 );
                  Listinfo_W_pattern->setStyleSheet( 0 );
                  Listinfo_R_pattern->setStyleSheet( 0 );

                  QMessageBox::information( this ,   "Justice"  , "DW12 valuw error , it isn't integer           "  , QMessageBox::Warning);
                 return;

               }

          }

    }


    Command_test_enable = 1;

    ui->pushButton->setEnabled( false );
    ui->pushButton_2->setEnabled( true );


    QFont font( "Courier New", 11 , QFont::Normal ) ;
    Listinfo->setFont( font );

    M.m_cnt1 = 0;
    M.m_cnt1_temp = 0;
    M.m_msg_list1.clear();


    log_mode_sw = 0;
    command_input_mode = true;
    m_control_scroll = false;

    M.m_cnt1 = 0;
    M.m_cnt1_temp = 0;
    M.m_msg_list1.clear();

    QStringList Com_argv_str;

    Listinfo_W_pattern->setWindowFlags( 0 );
    Listinfo_R_pattern->setWindowFlags( 0 );


    Listinfo -> setParent( NULL );

    Listinfo->show();

    Listinfo_W_pattern->show();
    Listinfo_R_pattern->show();
    Listinfo->setGeometry( 100 , 200 ,  500  , 515 );
    TextEdit_display_code->setGeometry(  570 , 200 ,  500   , 515 );

    Listinfo_W_pattern->setGeometry( ( 900 - 28 ) , 200 , ( 380 + 14 ) , 300 );
    Listinfo_R_pattern->setGeometry( ( 900 - 28 ) , 500 + 30 , ( 380 + 14 ) , 300 );


    command_input_mode = false;

    log_mode_sw = 1;
    m_control_scroll = true;

    // SaveSettings_KeyIn();

    ArgV_P1[ 0 ] = '0';
    ArgV_P2[ 0 ] = '1';

   TEST_COUNT = ui->TestCount_Edit->toPlainText();


    if( ui->checkBox->isChecked() )
      {
        K_Admin = '1';
        ASMIN = true;
        sprintf(admin_str , "%s" , "CMDTYPE=admin"  );

      }
    else
      {
         K_Admin = '0';
         ASMIN = false;
         sprintf(admin_str , "%s" , "CMDTYPE=nvme"  );

     }

    OPCODE = ui->OPCode->toPlainText();

    PARA1 = ui->DW1_Edit->toPlainText();
    PARA2 = ui->DW10_Edit->toPlainText();
    PARA3 = ui->DW11_Edit->toPlainText();
    PARA4 = ui->DW12_Edit->toPlainText();


    for( int i = 8 ; i < 300 ; i++ )
      buff[ i ] = 0;

   // --------------------- cmd.txt ------------------------

    FILE *fp;
    int len;

    char cmd_str[MAX_LINE];


   if((fp = fopen("cmd.txt","r")) == NULL)
    {

       QMessageBox::information( this,   "Justice"  ,  "          fail to read                     "   , QMessageBox:: Information);

       return ;
    }

   while(fgets(cmd_str, MAX_LINE,fp) != NULL)   // envp[ 1 ]
    {
       len = strlen(cmd_str );
       cmd_str[ len - 1 ] = '\0';
       strcat( buff , cmd_str );
    }


    fclose( fp);

   //---------------------------------------------------

    TEST_STOP = 0;

    time(& time_start_c  );


    days_c = 0;
    hours_c = 0;
    minutes_c = 0;
    sec_c = 0;
    time_diff_c = 0;

    timer_elapsed_c->stop();
    timer_elapsed_c->start( 1000 );   // 1 second


   //-----------------------------------------------------


    Com_argv_str << OPCODE.toStdString().c_str()<< PARA1.toStdString().c_str() << PARA2.toStdString().c_str() << PARA3.toStdString().c_str();

    M.m_msg_list1.clear();
    M.cmd1->execute(QString( file_name ), Com_argv_str );

    Listinfo->addItem( "" );

    mode_sw = 1;

    Listinfo_R_pattern->show();
    Listinfo_W_pattern->show();

    Listinfo->setGeometry( 100  , 200 , 650 , 515 );

    this->setGeometry(  780 , 280 - 40 , 487 , 380  );

    this->show();
    Listinfo->show();

    m_control_scroll = false;

    //---------------------------------------------------




    return ;

}


void KeyIn_test::on_checkBox_clicked()
{

}

void KeyIn_test::on_OPCode_destroyed()
{

}

void KeyIn_test::on_OPCode_destroyed(QObject *arg1)
{


}


void KeyIn_test::time_elapsed_c( void )
{


    time(&time_end_c);

    time_diff_c = time_end_c - time_start_c;
    sec_c = ( time_diff_c % 60 ) - 1 ;
    minutes_c = time_diff_c / 60;
    hours_c = minutes_c / 60;
    minutes_c = minutes_c % 60;
    days_c = hours_c / 24;
    hours_c = hours_c % 24;

    if( sec_c < 0 )
      sec_c = 0;

    char MS_Elapsed_c[ 100 ];
    sprintf( MS_Elapsed_c , "Elapsed : Day %02d  : Hour %02d  : Min %02d  : Sec %02d", days_c, hours_c, minutes_c, sec_c);
    //Elapsed_c_label->setText( MS_Elapsed_c );
    ui->label_7->setText( MS_Elapsed_c );



     if( TEST_STOP == 1 )
       {

         timer_elapsed_c->stop();


         Listinfo->setStyleSheet( 0 );
         Listinfo_W_pattern->setStyleSheet( 0 );
         Listinfo_R_pattern->setStyleSheet( 0 );


         ui->pushButton->setEnabled( true );
         ui->pushButton_2->setEnabled( false );

         M.cmd1->cancel();
         M.cmd1->freeable();

         Listinfo->scrollToBottom();

      }

}

/*
 *
int Input_string_product( void )
{



   return 1;

}

*/


int hex_digit_value(char x)
{
    switch (x)
    {
    case '0': return 1;
    case '1': return 1;
    case '2': return 1;
    case '3': return 1;
    case '4': return 1;
    case '5': return 1;
    case '6': return 1;
    case '7': return 1;
    case '8': return 1;
    case '9': return 1;
    case 'A':
    case 'a': return 1;
    case 'B':
    case 'b': return 1;
    case 'C':
    case 'c': return 1;
    case 'D':
    case 'd': return 1;
    case 'E':
    case 'e': return 1;
    case 'F':
    case 'f': return 1;
    default: return 0;
    }
}



