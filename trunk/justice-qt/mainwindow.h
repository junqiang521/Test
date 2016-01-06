#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "justice_cmd.h"
#include <QMessageBox>
#include <QKeyEvent>
#include <QAction>
//#include <keyin_test.h>

namespace Ui {
class MainWindow;

}

class MainWindow : public QMainWindow
{

    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    justice_cmd  *cmd1;
    justice_cmd  *cmd2;

    void wheelEvent(QWheelEvent* event);

    unsigned int m_cnt1 , m_cnt1_temp;
    QStringList m_msg_list1;


protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::MainWindow *ui;
    //KeyIn_test K_test;

private slots:

    void update_cmd1_ui( void );

private slots:


    void cmd1_started();
    void cmd1_stopped();
    void cmd2_started();
    void cmd2_stopped();

    void cmd1_info(QString msg);
    void cmd1_err(QString msg);
    void cmd2_info(QString msg);
    void cmd2_err(QString msg);

    void on_CLose_clicked();
    void on_stop1_clicked();

    void wbuf_info( void );
    void rbuf_info( void );



    void on_start1_clicked();
};


#endif // MAINWINDOW_H
