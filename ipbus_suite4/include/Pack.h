#pragma once
#include "gva.h"
#include "Thread.h"
#include "Buffer_pool.h"
#include "DataSend.h"
//#include "ReadOut.h"
//ProtectedQueue<Event*>* get_pack_Q(ReadOut*);
class Pack:public Thread
{
    private:
	Buffer* Buf;
        bool incom;
	DataSend* d_send;
        bool first;
    public:
	Pack();
	~Pack();
	void run();
};
void Exchange(uint32_t* p);
void Check(uint32_t* p,uint32_t N,uint32_t TIMES);
void Check_data(uint32_t* p,uint32_t N);
void Check_l1id(uint32_t* p);
