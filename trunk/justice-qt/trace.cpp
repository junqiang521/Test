#include "trace.h"
#include "ui_trace.h"
#include <QListWidget>
#include <qfile.h>
#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>
#include <QDir>
#include <QTextEdit>

QListWidget  *Commandinfo;

Trace::Trace(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Trace)
{
     ui->setupUi(this);

     this->setWindowTitle( "Trace" );
     this->setGeometry( 20 , 20 , 1030 , 670 );

     Commandinfo = new QListWidget();

     Commandinfo->setParent( this );
     Commandinfo->setGeometry( 20 , 20 , 990 ,560 );
     ui->save_button->move( 930 , 588 );
     ui->close_button->move( 930 , 625 );

}


void Trace::LOadHistory()
{

    Commandinfo->clear();

    QString file_Name = "/home/justice/justice-script/info/history.log";
    QFile loadfile( file_Name );
    loadfile.open(QIODevice::ReadOnly|QIODevice::Text);
    QTextStream in(&loadfile);
    QString line = in.readAll();
    QStringList loadlist = line.split(QRegExp("\n"), QString::SkipEmptyParts);
    loadfile.close();
    Commandinfo->addItems( loadlist  );

    Commandinfo->scrollToBottom();

}


Trace::~Trace()
{
    delete ui;
}

void Trace::on_pushButton_2_clicked()
{

}

void Trace::on_pushButton_clicked()
{


}

void Trace::on_save_button_clicked()
{

    QString Read_Name = "/home/justice/justice-script/info/history.log";
    QFile loadfile( Read_Name );
    loadfile.open(QIODevice::ReadOnly|QIODevice::Text);
    QTextStream in(&loadfile);

    QString line = in.readAll();
    QStringList loadlist = line.split(QRegExp("\n"), QString::SkipEmptyParts);

    QString file_Path;
    QString file_Name = QFileDialog::getSaveFileName(this, tr("Save as file") ,  file_Path  ,  tr("Images (*.js *)"));

    QFile newloads( file_Name );

    QListWidgetItem* item = Commandinfo->takeItem( Commandinfo->currentRow());
    delete item;

    newloads.open(QFile::WriteOnly|QFile::Truncate|QFile::Text);
    QTextStream out(&newloads);
    out << loadlist.join("\n");
    newloads.close();


}

void Trace::on_close_button_clicked()
{

     this->close();

}
