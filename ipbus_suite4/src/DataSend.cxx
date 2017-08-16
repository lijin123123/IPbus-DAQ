#include "DataSend.h"
#include <sys/socket.h>
#include <arpa/inet.h>
DataSend::DataSend()
{
}
DataSend::~DataSend()
{
	join();
}
void DataSend::run()
{
	prepare_socket();
	DEBUG_INFO("connection ok");
	Event* event;
	q_send->get(event);
	DEBUG_INFO("get event");
	unsigned int* length=new unsigned int;
	*length=(event->max_size())*4;   //bytes
	std::cout<<std::hex<<*length<<std::endl;
	send_data(sock,length,sizeof(unsigned int));
	send_data(sock,event->buf(),*length);
	delete length;
	delete event;
	while(1)
	{
		start_send();
		if(stop || stop_display)
		{
			DEBUG_INFO("prepare to exit");
			close(accept_sock);
			break;
		}
	}
	std::cout<<"data send thread exit"<<std::endl;
}
void DataSend::prepare_socket()
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
	addr_server.sin_port=htons(12453);
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
	accept_sock=accept(sock,0,0);
	if(accept_sock<0)
	{
		//log
		exit(1);
	}
}
void DataSend::start_send()
{
	bool quit=0;
	Event* event;
        while(q_send->size()==0)
	{
		if(stop || stop_display) 
		{
			quit=true;
			break;
		}
	}
	if (quit) return;
	q_send->get(event);
	//if(rand()%100<10)
	//{
		send_data(sock,event->buf()+3,(event->max_size())*4);
	//}
	delete event;
}

int DataSend::send_data(int sock,uint32_t* buffer,size_t size)
{
	size_t size_left=size,size_send=0;
	int ret;
	while(size_left>0)
	{
		ret=send(accept_sock,(void *)(buffer+size-size_left),size_left,0);
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
