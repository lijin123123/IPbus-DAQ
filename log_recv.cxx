#include "log_recv.h"
#include <iostream>
#include <errno.h>
void log_recv::prepare()
{
	struct sockaddr_in addr_server,addr_client;
	addr_server.sin_family= AF_INET;
    addr_server.sin_port= htons(5667);
	addr_server.sin_addr.s_addr=inet_addr("192.168.37.12");

	addr_client.sin_family=AF_INET;
	addr_client.sin_port=htons(5601);
	addr_client.sin_addr.s_addr=inet_addr("192.168.37.12");

	m_sock_log=socket(AF_INET,SOCK_STREAM,0);

	if(bind(m_sock_log,(struct sockaddr*)&addr_client,sizeof(struct sockaddr))<0)
	{
        std::cout<<"log thread bind error"<<std::endl;
	}
	if(::connect(m_sock_log,(struct sockaddr*)&addr_server,sizeof(struct sockaddr))<0)
	{
		//log
	std::cerr<<strerror(errno)<<std::endl;
        std::cout<<"log thread connect error"<<std::endl;
	}
}
void log_recv::run()
{
	stop =false;
	prepare();
	loginfo log;
	while(!stop)
	{
        int ret=recvn(m_sock_log,&log,sizeof(loginfo));
	if(ret==0)
	{
		continue;
	}
        QVariant DataVar;
        DataVar.setValue(log);
        emit log_display(DataVar);
	}
	::close(m_sock_log);
	std::cout<<"log thread exit"<<std::endl;
}
void log_recv::onsigstop()
{
	stop=true;
}
int log_recv::recvn(int sock,loginfo* log,size_t size)
{
        size_t size_left=size,size_recv=0;
        int ret;
        while(size_left>0)
        {
            ret=recv(sock,(void *)(log+size-size_left),size_left,0);
            if(ret>0)
            {
                size_recv+=ret;
                size_left-=ret;
            }
            else
            {
               // std::cout<<"ret="<<ret<<"recv error!!!!!!"<<std::endl;
		return 0;
                //log

            }
        }
        return size_recv;
}
