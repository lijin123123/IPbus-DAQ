#ifndef LOG_RECV_H
#define LOG_RECV_H

#include <QThread>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "gva.h"
/*
enum LEVEL{INFO,WARNNING,ERROR,FATAL};
struct loginfo
{
	char log[100];
	time_t logtime;
	LEVEL level;
};
*/
class log_recv:public QThread
{
	Q_OBJECT
signals:
	void log_display(QVariant dataVar);
private slots:
	void onsigstop();
protected:
	void run();
	int recvn(int sock,loginfo* log,size_t size);
	void prepare();
private:
	int m_sock_log;
    bool stop;
		
};



#endif // LOG_RECV_H
