#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<unistd.h>
#include<stdlib.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    char path[]="node";
    char * argv[3];
    char * envp[2];

    argv[0] = path;
    argv[1] = "/home/gong/qttest/1.js";
    argv[2] = NULL;

    envp[0] = path;
    envp[1] = NULL;

    execvpe("node",argv,envp);
}
