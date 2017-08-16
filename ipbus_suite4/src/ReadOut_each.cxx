#include "ReadOut_each.h"
#define dummy 1
ReadOut_each::ReadOut_each(int n):board_n(n)
{
	check_enable=false;
	std::cout<<"enter readout_each"<<std::endl;
}
void ReadOut_each::run()
{
	int thread_arg=board_n;
	//ReadOut* r=ReadOut::instance();
	//std::vector<ProtectedQueue<Event*>*> q_r_v=get_read_Q(r);
	//ProtectedQueue<Event*>* q_r=q_r_v[thread_arg];
	ProtectedQueue<Event*>* q_r=q_readout[thread_arg];
	ValVector<uint32_t> mem;
	int trytimes;
	Event* event;
	pthread_mutex_lock(&daq_status_mutex);
	while(status==STARTING)
	{
		pthread_cond_wait(&daq_status_cv,&daq_status_mutex);	
	}
	pthread_mutex_unlock(&daq_status_mutex);
	bool first_packet=true;
	while(!stop)
	{
		event=new Event(event_size);
		uint32_t* p=event->buf();
		/*
		   if(ReadoutFifo[thread_arg].size()==0)
		   {
		   __sync_fetch_and_add(&stop,1);
		   break;  
		   }
		   */
		mem=HW[thread_arg]->getNode("MEM").readBlock(event_size);
		try
		{
			HW[thread_arg]->dispatch();
			trytimes=0;
			for(uint32_t i=0;i<event_size;i++)
			{
				*(p+i)=mem[i];
				//std::cout<<*(p+i)<<" ";
			}
			//std::cout<<std::endl;
		}
		catch(std::exception& e)
		{
			trytimes++;
			usleep(100);
			if(trytimes>100000)
			{
				__sync_fetch_and_add(&stop,1);
				std::cout<<"Readout set stop 1"<<std::endl; 
			}
			//log
			std::cout<<"read err msg:"<<e.what()<<std::endl;
			continue;
		}
		if(check_enable && !dummy)
		{
			check_header();
			check_length();
			check_tailer();
			check_trigger();
		}
		q_r->put(event);
	}
	std::cout<<"single readout exit"<<std::endl;
	return ;//pthread_exit(NULL);
}
int ReadOut_each::get_board_n()
{
	return board_n;
}

ReadOut_each::~ReadOut_each()
{
	join();
}

inline bool ReadOut_each::check_header()
{
}

inline bool ReadOut_each::check_tailer()
{
}

inline bool ReadOut_each::check_length()
{
}

inline bool ReadOut_each::check_trigger()
{
	if(first_packet)
	{
		actual_l1id = *p;
		expected_l1id = actual_l1id+1;
		first_packet =false;
	}
	else
	{
		actual_l1id = *p;
		if(actual_l1id!=expected_l1id)
		{
			//log
			// __sync_fetch_and_add(&stop,1);
			// return false;
			expected_l1id= actual_l1id+1;
		}
		else
		{
			expected_l1id++;
		}
	}
	return true;
}
