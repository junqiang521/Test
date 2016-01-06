#ifndef BUFFER_SETTING_H
#define BUFFER_SETTING_H

#include <QDialog>

namespace Ui {
class buffer_setting;
}

class buffer_setting : public QDialog
{
    Q_OBJECT

public:
    explicit buffer_setting(QWidget *parent = 0);
    ~buffer_setting();

     int buffer_len;
     void loadSettings();

      void WriteSettings();

private slots:
     void on_buttonBox_accepted();

     void on_pushButton_clicked();

     void on_lineEdit_cursorPositionChanged(int arg1, int arg2);

private:
    Ui::buffer_setting *ui;




};

#endif // BUFFER_SETTING_H
