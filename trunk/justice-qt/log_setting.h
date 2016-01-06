#ifndef LOG_SETTING_H
#define LOG_SETTING_H

#include <QDialog>

namespace Ui {
class Log_setting;
}

class Log_setting : public QDialog
{
    Q_OBJECT

public:
    explicit Log_setting(QWidget *parent = 0);
    ~Log_setting();

    bool check_save_log;
    bool check_change_loge_name;
    QString log_name_line;

private slots:

    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

    void on_log_name_line_returnPressed();

    void on_log_name_line_cursorPositionChanged(int arg1, int arg2);

private:
    Ui::Log_setting *ui;


};

#endif // LOG_SETTING_H
