#ifndef SEL_DEV_H
#define SEL_DEV_H

#include <QFileDialog>
#include <QMainWindow>
#include <QLabel>
#include <QDialog>
#include "mainwindow.h"
#include <QApplication>

namespace Ui {
class sel_dev;
}

class sel_dev : public QDialog
{
    Q_OBJECT

public:

    explicit sel_dev(QWidget *parent = 0);
    void WriteSettings();
    ~sel_dev();

private slots:

    void on_OK_clicked();
    void on_pushButton_4_clicked();
    void on_tableView_clicked(const QModelIndex &index);
    void on_OK_clicked(bool checked);
    void on_OK_released();
    void on_Close_clicked();

private:

    QStatusBar *StatusBar;
    Ui::sel_dev *ui;
    MainWindow M;

};

#endif // SEL_DEV_H















