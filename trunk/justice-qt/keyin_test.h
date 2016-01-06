#ifndef KEYIN_TEST_H
#define KEYIN_TEST_H

#include <QDialog>
#include <mainwindow.h>
#include <QTime>


namespace Ui {
class KeyIn_test;
}

class KeyIn_test : public QDialog
{
    Q_OBJECT

public:
    explicit KeyIn_test(QWidget *parent = 0);
    ~KeyIn_test();

private slots:
    void on_Save_pushButton_clicked();

    void on_Load_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_checkBox_clicked();

    void on_OPCode_destroyed();

    void on_OPCode_destroyed(QObject *arg1);

    void time_elapsed_c( void );

private:
    Ui::KeyIn_test *ui;
    QTimer *timer_elapsed_c;

    MainWindow M;


};

#endif // KEYIN_TEST_H
