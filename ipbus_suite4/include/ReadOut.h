#pragma once
#include "Thread.h"
#include "Buffer_pool.h"
#include <vector>
#include "gva.h"
#include "Pack.h"
#include "SaveToDisk.h"
#include "protected_queue.h"
#include "Event.h"
#include "ReadOut_each.h"
class ReadOut:public Thread
{
    //friend ProtectedQueue<Event*>* get_pack_Q(ReadOut*);
    //friend ProtectedQueue<Event*>* get_save_Q(ReadOut* );
    //friend std::vector<ProtectedQueue<Event*>*> get_read_Q(ReadOut* );
    //friend std::vector<ProtectedQueue<Event*>*> get_readoutQ_vector();
    private:
        static ReadOut* m_self; 
	std::vector<Buffer*> Buf; 
	Pack* m_pack;
	SaveToDisk* m_save;
        std::vector<ReadOut_each*> p_readout;
	//std::vector<ProtectedQueue<Event*>*> q_readout;
	//ProtectedQueue<Event*>* q_pack;
	//ProtectedQueue<Event*>* q_send;
    public:
        static ReadOut* instance();
	ReadOut();
	~ReadOut();
        bool GetQue(ProtectedQueue<Event*>* q_r,int i);
        void run();
};
