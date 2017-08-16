#include "ReadOut.h"
#include <queue>
#include <semaphore.h>
#include <cstdio>
#include <iostream>
#include "ReadOut_each.h"
/*
ProtectedQueue<Event*>* get_pack_Q(ReadOut* r)
{
	return r->q_pack;
}
ProtectedQueue<Event*>* get_save_Q(ReadOut* r)
{
	return r->q_save;
}
std::vector<ProtectedQueue<Event*>*> get_read_Q(ReadOut* r)
{
	return r->q_readout;
}
*/
ReadOut::ReadOut()
{
	//save_enable=true;
        //send_display =true;
}
ReadOut* ReadOut::m_self=NULL;
ReadOut* ReadOut::instance()
{
    if(m_self==NULL)
    {
	m_self=new ReadOut;
    }
    return m_self;
}
void ReadOut::run()
{
    status = STARTING;
    for(int i=0;i<board_num;i++)
    {
	q_readout.push_back(new ProtectedQueue<Event*>(1000));
    }
    q_pack = new ProtectedQueue<Event*>(1000);
    q_send = new ProtectedQueue<Event*>(1000);

    std::cout<<"enter ReadOut thread"<<std::endl;
    m_pack=new Pack();
    m_save=new SaveToDisk();
    m_pack->start();
    if(save_enable)
    {
    m_save->start();
    }
    ReadOut_each* re;
    for(uint32_t i=0;i<board_num;i++)
    {
	re=new ReadOut_each(i);
	re->start();
	p_readout.push_back(re);
    }
    status=RUNNING;
    pthread_cond_broadcast(&daq_status_cv);
    for(int i=0;i<board_num;i++)
    {
	delete p_readout[i];
    }
    delete m_pack;
    if(save_enable)
    {
    delete m_save;
    }
    //sleep(10000);
    std::cout<<"ReadOut thread exit"<<std::endl;
    log_create("ReadOut thread exit",INFO,time(NULL));
    return;//pthread_exit(NULL);
}
ReadOut::~ReadOut()
{
    join();
}
