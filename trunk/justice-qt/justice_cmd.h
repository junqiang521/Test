#ifndef JUSTICE_CMD_H
#define JUSTICE_CMD_H

#include <QObject>
#include <QString>
#include <QStringList>

//#include <pcie.h>

//note. The class only safe in GUI main thread.
//And Always use Qt::QueuedConnection to connect signals
//don't free cmd when there is some thing executing
//use deferred delelte combine with stopped signal/cancel/freeable


class JSexecutor;

class justice_cmd : public QObject
{
    Q_OBJECT
public:
    explicit justice_cmd(QObject *parent = 0);
    ~justice_cmd();

    char *m_wbuf;  // Modify by Alwin
    char *m_rbuf;

    int m_wshmid;
    int m_rshmid;

    void justice_cmd_buf( void );  // Add by Alwin for GUI

    int execute(const QString &path, const QStringList &args);//0 success. < 0 failed. if success, will started/info or err/stopped.
    void cancel();

    bool freeable() {return m_executor == NULL;}   // TEST_STOP


    char *get_wbuf(int size) {
        (void)size;
        return m_wbuf;
    }


    void put_wbuf() {}


    char *get_rbuf(int size) {
        (void)size;
        return m_rbuf;
    }


    void put_rbuf() {}


signals:
    void info(QString msg);
    void err(QString msg);
    void started();
    void stopped();




private:

    // char *m_wbuf;
    // char *m_rbuf;

     //int m_wshmid;
     //int m_rshmid;

    QString m_info_chnl;
    QString m_info_key;
    QString m_rbuf_key;
    QString m_wbuf_key;
    JSexecutor *m_executor;

private slots:
    void slot_stopped() {m_executor = NULL;}




friend class JSexecutor;
};




#endif // JUSTICE_CMD_H
