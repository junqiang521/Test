#ifndef TRACE_H
#define TRACE_H

#include <QDialog>

namespace Ui {
class Trace;
}

class Trace : public QDialog
{
    Q_OBJECT

public:
    explicit Trace(QWidget *parent = 0);
    ~Trace();

     void LOadHistory();

private slots:

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_save_button_clicked();

    void on_close_button_clicked();

private:
    Ui::Trace *ui;




};

#endif // TRACE_H
