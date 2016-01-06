#include "justice_cmd.h"
#include <QThreadPool>
#include <QRunnable>
#include <QDebug>
#include "hiredis/hiredis.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/wait.h>
#include <assert.h>
#include <unistd.h>
#include <stdint.h>
#include <QTextEdit>
#include <QListWidget>

#define SHARED_MEM_SIZE (64 * 1024 * 1024)
#define CMM_LEN 64
#define MAX_LINE 1024

extern unsigned char buf_W[ SHARED_MEM_SIZE ] , buf_R[ SHARED_MEM_SIZE ];
extern int buffer_length;
extern bool TEST_STOP;
extern QListWidget  *Listinfo_W_pattern;
extern QListWidget  *Listinfo_R_pattern;
extern QTextEdit    *TextEdit_display_code;
extern QListWidget  *Listinfo;
extern int str_length;
extern int str_length_temp;
extern bool m_control_scroll;
extern bool command_input_mode;
extern bool Command_test_enable;
extern char buff[300];

extern QString  K_Test_count;
extern QString  K_Admin;

extern bool ASMIN;
extern QString TEST_COUNT;

char admin_str[ 100 ];
char Count_str[ 100 ];

class JSexecutor : public QRunnable
{

    static void execute_js(const QString &js, const QStringList &args, const justice_cmd &cmd) {
        QByteArray r_key = cmd.m_rbuf_key.toUtf8();
        QByteArray w_key = cmd.m_wbuf_key.toUtf8();
        QByteArray info_key = cmd.m_info_key.toUtf8();
        QString TestCount;
        int cnt = args.length();
        QByteArray ref_args[cnt];
        for (int i = 0; i != cnt; i++)
            ref_args[i] = args[i].toUtf8();

        char *argv[3 + cnt];
        char *envp[10];
        char path[] = "node";

        QByteArray script = js.toUtf8();
        argv[0] = path;
        argv[1] = script.data();
        for (int i = 0; i != cnt; i++)
            argv[2 + i] = ref_args[i].data();
        argv[2 + cnt] = NULL;

        envp[0] = r_key.data();
        envp[1] = w_key.data();
        envp[2] = info_key.data();
        envp[3] = NULL;


        if( Command_test_enable == 1 )
        {

            argv[0] = path;
            //argv[1] = "/home/justice/justice-script/execute/1.js";
            argv[1] = "/home/justice/justice-script/command/blocks.js";
            argv[2] = NULL;

            envp[3] = admin_str;
            envp[4] = buff;

            sprintf( Count_str , "TEST_CNT=%s" , TEST_COUNT.toStdString().c_str() );

            envp[5] = Count_str ;
            envp[6] = NULL;

        }


    if( Command_test_enable == 1 )
      {

        qDebug() << "argv";
        for (int i = 0; i < 3; i++)
           qDebug() << argv[i];

        qDebug() << "envp";
        for (int i = 0 ; i < 7 ; i++)
           qDebug() << envp[i];

      }
     else
      {

         qDebug() << "argv";
         for (int i = 0; i != 2 + cnt; i++)
             qDebug() << argv[i];

         qDebug() << "envp";
         for (int i = 0; i != 3; i++)
             qDebug() << envp[i];

      }



        int status =  execvpe( path, argv, envp);

        return ;
    }


private:
    justice_cmd &m_cmd;
    QString m_path;
    QStringList m_args;

public:
    JSexecutor(justice_cmd &cmd, const QString &path, const QStringList &args) : m_cmd(cmd), m_path(path), m_args(args) {


    }

    void run() {


        pid_t pid = -1;
        void *reply = NULL;
        const char *err_str = NULL;

        redisContext *c = redisConnect("127.0.0.1", 6379);
        if (!c || (c->err != 0))
                goto ERR_CHNL_CONN;

        reply  = redisCommand(c, "SUBSCRIBE %s", qUtf8Printable(m_cmd.m_info_chnl));
        if (!reply)
            goto ERR_CHNL_SUB;
        freeReplyObject(reply);

        pid = fork();
        if (pid == -1)
            goto ERR_FORK;
        else if (pid == 0) {
            execute_js(m_path, m_args, m_cmd);
            exit(0);

            return ;
        }


        emit m_cmd.started();
        while (redisGetReply(c, &reply) == REDIS_OK)
        {

            struct redisReply *r = (struct redisReply *)reply;
            int ended = 0;
            if (r->type == REDIS_REPLY_ARRAY && r->elements == 3) {
                emit m_cmd.info(r->element[2]->str);

                if (strcmp("end", r->element[2]->str) == 0)
                  {
                     ended = 1;
                     TEST_STOP = 1;  //  Question 1 :  Need a extern visable to know program end or error
                                     //             :  So i use TEST_STOP control GUI "elapsed time" Start/ Stop
                  }


                // add by Alwin
                //str_length_temp = strlen( r->element[2]->str  );  // if message less 5 chara and elapsed time more than 2 second
                //if( str_length_temp > str_length )                //  elpased time stop
                   {
                      //str_length = str_length_temp;
                      //str_length_temp = 0;
                   }


               emit m_cmd.justice_cmd_buf();  // Add by Alwin for GUI


            }
            freeReplyObject(reply);
            if (ended)
                break;
        }


        redisFree(c);
        kill(pid, SIGKILL);
        emit m_cmd.stopped();
        waitpid(pid,  NULL, 0);

       if( command_input_mode == 0 )
          m_control_scroll = false; // add by Alwin

        return;

ERR_FORK:
        err_str = "JS fork  err.";


ERR_CHNL_SUB:
        err_str = err_str == NULL ? "JS message channel subscribe err." : err_str;


ERR_CHNL_CONN:
        err_str = err_str == NULL ? "JS message channel conn err." : err_str;


        if  (c)
            redisFree(c);
        emit m_cmd.started();
        emit m_cmd.err(err_str);
        emit m_cmd.stopped();


        return;
    }
};

justice_cmd::justice_cmd(QObject *parent) : QObject(parent)
{

    m_rshmid = shmget(IPC_PRIVATE, SHARED_MEM_SIZE, 0644 | IPC_CREAT);
    m_wshmid = shmget(IPC_PRIVATE, SHARED_MEM_SIZE, 0644 | IPC_CREAT);

    assert(m_rshmid != -1);
    assert(m_wshmid != -1);

    m_rbuf = ( char *)shmat(m_rshmid, 0, 0);
    m_wbuf = ( char *)shmat(m_wshmid, 0, 0);

    assert(m_rbuf);
    assert(m_wbuf);

    m_executor = NULL;

    m_info_chnl = QString("info") + QString("_pid") + QString::number((uintptr_t)getpid()) + QString("_ptr") + QString::number((uintptr_t)(void *)this);
    m_info_key = QString("JT_INFO_CHNL=") + m_info_chnl;
    m_rbuf_key = QString("JT_RD_BUFF=") + QString::number(m_rshmid);
    m_wbuf_key = QString("JT_WT_BUFF=") + QString::number(m_wshmid);

    connect(this,  SIGNAL(stopped()), this, SLOT(slot_stopped()), Qt::QueuedConnection);


}


void justice_cmd::justice_cmd_buf()
{

   // ---------------------------------------------------------------------

   //m_wbuf = get_wbuf( ( buffer_length * 512 )  );
   //m_rbuf = get_rbuf( ( buffer_length * 512 )  );

  /*
    for( int i = 0 ; i < (buffer_length * 512 )   ; i++ )
      {
             printf(" %02x  ",   m_rbuf[  i  ]         ) ;

       }

    printf( "\n ") ;

    */
  // ------------------------------------------------------------------------

    memcpy( buf_R , m_rbuf , ( buffer_length * 512 ) );
    memcpy( buf_W , m_wbuf , ( buffer_length * 512 ) );

}


justice_cmd::~justice_cmd()
{

    assert(m_rshmid != -1);
    assert(m_wshmid != -1);
    assert(m_rbuf);
    assert(m_wbuf);

    assert(!m_executor);//Make sure when delete, there is no outstanding cmd.

    shmdt(m_rbuf);
    shmdt(m_wbuf);
    shmctl(m_rshmid, IPC_RMID, 0);
    shmctl(m_wshmid, IPC_RMID, 0);


}


int justice_cmd::execute(const QString &path, const QStringList &args)
{
    if (m_executor)
        return -1;
    m_executor = new JSexecutor(*this, path, args);
    if (!m_executor)
        return  -1;
    QThreadPool::globalInstance()->start(m_executor);

    return 0;
}

void justice_cmd::cancel()
{

    if (!m_executor)
        return;

    redisContext *c = redisConnect("127.0.0.1", 6379);
    if (!c)
        return;
    if (c->err != 0) {
        redisFree(c);
        return;
    }

    void *reply  = redisCommand(c, "PUBLISH %s %s", qUtf8Printable(m_info_chnl), "end");
    if (reply)
        freeReplyObject(reply);
    redisFree(c);


}

