#pragma once
#include "gva.h"
#include "Thread.h"
#include <sys/socket.h>
#include <arpa/inet.h>


class LogSend:public Thread
{
	public:
		void run();
		void prepare_sock();
                int send_log(int sock,uint32_t* buffer,size_t size);
		~LogSend();
	private:
		int sock;
		int log_sock;
};

