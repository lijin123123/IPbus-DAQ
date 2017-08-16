#ifndef DATA_DISPLAY_H
#define DATA_DISPLAY_H

#include <QThread>
#include <sys/socket.h>
#include <arpa/inet.h>

class mythread:public QThread
{
    Q_OBJECT
signals:
    void drawsignal(int len);
private slots:
    void onsigstopdisplay();
protected:
    void run();
    int recvn(int sock,u_int32_t* m_buffer,size_t size);
    void prepare();
private:
    int m_sock_data;
    int length;//words
    bool stop_display;
};

#endif // DATA_DISPLAY_H
