#include "SaveToDisk.h"
#include <queue>
#include <pthread.h>
#include <fstream>
SaveToDisk::SaveToDisk()
{
}
void SaveToDisk::run()
{
	std::cout<<"enter Save thread"<<std::endl;
	int trytimes=0;
	int size=0;
	//ReadOut* r=ReadOut::instance();
	//ProtectedQueue<Event*>*q_p=get_pack_Q(r);
	pthread_mutex_lock(&daq_status_mutex);
	while(status==STARTING)
	{
		pthread_cond_wait(&daq_status_cv,&daq_status_mutex);	
	}
	pthread_mutex_unlock(&daq_status_mutex);
        header h;
	//sem_wait(&p_sem);   
	while(1)
	{
		Event* e;
		while(q_pack->size()==0)
		{
		//	DEBUG_INFO("SAVE00000");
			if(stop==1) break;
		}
                if(stop!=1)
                {
		q_pack->get(e);
		uint32_t* p=e->buf();
		const char* filename="./data";
		//text file
		std::ofstream save(filename,std::ios::app);
		if(!save)
		{   
			std::cout<<"error open"<<std::endl;
		}
                uint32_t length;   
                if(header_enable)
		{
			length=sizeof(h)/4+event_size*board_num;
		}
		else
		{
			length=event_size*board_num;
		}
                for(uint32_t i=0;i<length;i++)
                {
			save<<std::hex<<*(p+i)<<" ";
		}
		//std::cout<<"record"<<std::endl;   
		save<<"\n";
		save<<std::flush;
		//binary file
		/*
		   uint32_t N=64;
		   std::ofstream save(filename,std::ios::binary|std::ios::app);
		   if(!save)
		   {
		   std::cout<<"error open"<<std::endl;
		   }
		   save.write((const char *)p,4*N);
		   */

		/* 
		   pthread_mutex_lock(&mutex);
		   p_sem--;
		   pthread_mutex_unlock(&mutex);
		   */
		delete e;
                save.close();
		}
		else 
		{
                        break;
		}
	}
	std::cout<<"save thread exit"<<std::endl;
	return ;//pthread_exit(NULL);
}
SaveToDisk::~SaveToDisk()
{
	join();
}
