
#ifndef PCIE_H
#define PCIE_H

#include <QStandardItemModel>
#include <pcie_set_script.h>
#include <QFileSystemModel>
#include <QtCore>
#include <QStatusBar>
#include <mainwindow.h>
#include "justice_cmd.h"
#include <QEvent>
#include <log_setting.h>
#include <sel_dev.h>
#include <QDialog>
#include <buffer_setting.h>
#include <trace.h>
#include <keyin_test.h>

namespace Ui {
class PCIE;
}

class PCIE : public QDialog
{
    Q_OBJECT

public:
    explicit PCIE(QWidget *parent = 0);
    ~PCIE();

   bool install_jdriver( void );
   bool Get_DUT_information( void );
   bool Get_Driver_version( void );
   bool Auto_argv_run( void );
   bool Justice_Command_input( QString filePath_com , QStringList Argv_str );
   void loadSettings();
   void command_input_help( void );
   void keyPressEvent( QKeyEvent * event );
   void wheelEvent(QWheelEvent* event);
   void closeEvent(QCloseEvent *event);




   QStatusBar *StatusBar;


private slots:

   void change( void );
   void slot_menu_action_NEW( void );
   void slot_menu_action_OPEN( void );
   void slot_menu_action_SAVE( void );
   void slot_menu_action_SAVEAS( void );
   void slot_menu_action_CLOSE( void );
   void slot_menu_action_EXIT( void );
   void slot_menu_action_BUFFER_CLEAR( void );
   void slot_menu_action_BUFFER_FILL( void );
   void slot_menu_action_BUFFER_COMPARE_( void );
   void slot_menu_action_BUFFER_COPY_( void );
   void slot_menu_action_BUFFER_SAVE_BIN( void );
   void slot_menu_action_TEST_STATUS( void );
   void slot_menu_action_TOOLA( void );
   void slot_menu_action_TOOLB( void );
   void slot_menu_action_TOOLLOG_SETTING( void );
   void slot_menu_action_TOOL_BUFFER_SETTING( void );
   void slot_menu_action_TOOL_POWER_SETTING( void );
   void slot_menu_action_TOOL_DATABASE_SETTING( void );
   void slot_menu_action_HELP_COMINPUT_F1( void );
   void slot_menu_action_HELP_ABOUT_JUSTICE( void );

  void P_parameter( char *str_loc , int cmd_len ,int  com_num );
  void Record_log( char *str );
  void Start_Record_log( void );
  void Input_log( char *str );
  void Start_Input_log( void );
  void time_elapsed(  void  );
  void LoadScriptContext( QString  script_name );
  void on_lineEdit_cursorPositionChanged(int arg1, int arg2);
  void on_lineEdit_editingFinished();
  void on_lineEdit_textEdited(const QString &arg1);
  void on_lineEdit_returnPressed();
  void on_pushButton_21_clicked();
  void on_pushButton_22_clicked();
  void on_pushButton_28_clicked();
  void on_label_13_linkActivated(const QString &link);
  void on_Run1_clicked();
  void on_Stop1_clicked();
  void on_Run1_pressed();
  void on_Run1_released();
  void on_Run1_clicked(bool checked);
  void on_Clear_clicked();
  void on_Clear_pressed();

  void on_Idfy1_Button_clicked();
  void on_Idfy0_Button_pressed();
  void on_Idfy0_Button_clicked();
  void on_PCI_Button_clicked();
  void on_CTRL_Button_clicked();
  void on_Histoty_button_clicked();

  void on_Command_button_clicked();

  void Run2_clicked( void );
  void Stop2_clicked( void );

private:

    Ui::PCIE *ui;
    QTimer *timer_elapsed;
    QFileSystemModel *dirModel;
    QFileSystemModel *fileModel;
    Log_setting  Log_s;
    buffer_setting Buffer_s;
    MainWindow M;
    sel_dev  S_dev;
    Trace    T_com;

    KeyIn_test K_test;




};



#endif // PCIE_H












