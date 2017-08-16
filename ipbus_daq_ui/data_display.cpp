#include "data_display.h"
#include <iostream>
extern u_int32_t* m_buffer;


void mythread::run()
{
    prepare();
    stop_display=false;
    recvn(m_sock_data,m_buffer,4);
    length=(*m_buffer)/4;
    while(!stop_display)
    {
        
        int ret=recvn(m_sock_data,m_buffer,length*4);
	if(ret==0)
	{
	continue;
	}
        emit drawsignal(length);
    }
    ::close(m_sock_data);
    std::cout<<"data recv thread exit"<<std::endl;
}
void mythread::onsigstopdisplay()
{
    std::cout<<"here in zi thread"<<std::endl;
    stop_display=true;
}

void mythread::prepare()
{
    struct sockaddr_in addr_server,addr_client;
    addr_server.sin_family= AF_INET;
    addr_server.sin_port= htons(12453);
    addr_server.sin_addr.s_addr=inet_addr("192.168.37.12");

    addr_client.sin_family=AF_INET;
    addr_client.sin_port=htons(5666);
    addr_client.sin_addr.s_addr=inet_addr("192.168.37.12");

    m_sock_data=socket(AF_INET,SOCK_STREAM,0);

    if(bind(m_sock_data,(struct sockaddr*)&addr_client,sizeof(struct sockaddr))<0)
    {
        std::cout<<"data recv thread bind error"<<std::endl;
    }
    if(::connect(m_sock_data,(struct sockaddr*)&addr_server,sizeof(struct sockaddr))<0)
    {
        //log
        std::cout<<"data recv thread connect error"<<std::endl;
    }
}

int mythread::recvn(int sock,u_int32_t* buffer,size_t size)
{
        size_t size_left=size,size_recv=0;
        int ret;
        while(size_left>0)
        {
            ret=recv(sock,(void *)(buffer+size-size_left),size_left,0);
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
