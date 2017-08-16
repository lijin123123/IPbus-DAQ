#include "LogSend.h"
#include <cstring>
void LogSend::prepare_sock()
{
	sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock<0)
	{
		std::cout<<"%%%%%%%%%%%"<<std::endl;
		//log
		exit(1);
	}
	struct sockaddr_in addr_server;
	addr_server.sin_family=AF_INET;
	addr_server.sin_port=htons(5667);
	addr_server.sin_addr.s_addr = inet_addr("192.168.37.12");

	if(bind(sock,(struct sockaddr*)&addr_server,sizeof(struct sockaddr))<0)
	{
		//log
		std::cout<<"bind error"<<std::endl;
		exit(1);
	}
	if(listen(sock,100)<0)
	{
		//log
		exit(1);
	}
	log_sock=accept(sock,0,0);
	if(log_sock<0)
	{
		//log
		exit(1);
	}
}
void LogSend::run()
{
	std::cout<<"logsend thread"<<std::endl;
	prepare_sock();
	loginfo log ;
	//char* c="log thread connected";
	//LEVEL l=INFO;
	//log.logtime=time(NULL);
	log_create("log thread cxonnected",INFO,time(NULL));
	while(!stop)
	{
		loginfo l;
                q_log->get(l);
	//	DEBUG_INFO("waiting ....");
                send_log(log_sock,(uint32_t*)&l,sizeof(loginfo));
	}
	close(log_sock);
	std::cout<<"log send thread exit"<<std::endl;
}
LogSend::~LogSend()
{
	join();
}
void log_create(std::string s,LEVEL l,time_t t)
{
	loginfo log;
	log.level = l;
	log.logtime=t;
        strcpy(log.log,s.c_str());
	q_log->put(log);
}
int LogSend::send_log(int sock,uint32_t* buffer,size_t size)
{
	size_t size_left=size,size_send=0;
	int ret;
	while(size_left>0)
	{
		ret=send(sock,(void *)(buffer+size-size_left),size_left,0);
		if(ret>0)
		{
			size_send+=ret;
			size_left-=ret;
		}
		else
		{
			std::cout<<"HHHHHHHHHHHHHH"<<std::endl;
			//log

		}
	}
	return size_send;
}
