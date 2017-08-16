#pragma once
#include "Thread.h"
#include "gva.h"
class DataSend:public Thread
{
	private:
		int sock;
		int accept_sock;
	public:
		DataSend();
		~DataSend();
		void run();
                void prepare_socket();
                void start_send();
		int send_data(int, uint32_t*, size_t);
	
};
