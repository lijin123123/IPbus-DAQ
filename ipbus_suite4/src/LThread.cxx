#include "LThread.h"
#include "gva.h"
#include <sys/socket.h>
#include <arpa/inet.h>
void LThread::prepare_socket()
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
	addr_server.sin_port=htons(12345);
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
void LThread::run()
{
	prepare_socket();
	log_create("listen thread cxonnected",ERROR,time(NULL));
	CMDTYPE cmd;
	while(1)
	{
		//	std::cout<<"looplooploop"<<std::endl;
		int ret=recv(accept_sock,&cmd,sizeof(cmd),0);
		//        std::cout<<"ret="<<ret<<"cmd="<<cmd<<std::endl;
		if(ret<0)
		{
			//log
			std::cout<<"recv cmd error"<<std::endl;
			close(accept_sock);			
		}
		else
		{
			switch (cmd)
			{
				case CONFIG:
					//std::cout<<"recv config"<<std::endl;
					do_lock();break;
				case START:
					{
						//std::cout<<"recv start"<<std::endl;
						do_lock();break;
					}
				case SAVE:
					{
						__sync_fetch_and_add(&save_enable,1);break;
					}
				case DISPLAY:
					{
						__sync_fetch_and_add(&display_thread,1);break;
					}
				case START_DISPLAY:
					{
						std::cout<<"recv start_display"<<std::endl;
						__sync_fetch_and_add(&start_display,1);break;
					}
				case STOP_DISPLAY:
					{
						std::cout<<"recv stop display"<<std::endl;	
						__sync_fetch_and_add(&stop_display,1);break;
					}
				case STOP:
					    std::cout<<"recv stop"<<std::endl;
					__sync_fetch_and_add(&stop,1);do_lock();break;
				case DISCONNECT:
					//std::cout<<"recv disconnect"<<std::endl;
					close(accept_sock);close(sock);return;
				default:
					//std::cout<<"recv default"<<std::endl;
					continue;break;
			}
		}
	}
}
void LThread::do_lock()
{
	done=0;
	pthread_mutex_lock(&mutex);
	sem_post(&l_r_sem);
	while(done==0)
	{
		pthread_cond_wait(&cv,&mutex);
	}
	pthread_mutex_unlock(&mutex);
}
