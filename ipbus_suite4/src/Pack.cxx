#include "Pack.h"
#include <pthread.h>
#include <cstring>
#include <iostream>
uint32_t* des;
Pack::Pack()
{
	std::cout<<board_num<<std::endl;
	header_enable=true;
	first = true;

}
Pack::~Pack()
{
	join();
	std::cout<<"Pack destruct"<<std::endl;
}
void Pack::run()
{
	//uint32_t TIMES=0;
	//__sync_fetch_and_add(&start_display,1);
	std::cout<<"enter pack "<<std::endl;
	header h; 
	if(header_enable)
	{
		h.tag=0xaa1234aa;
		h.event_num=0x00;
		h.length = event_size*board_num;   //words 
	}
	incom =false;
	pthread_mutex_lock(&daq_status_mutex);
	while(status==STARTING)
	{
		pthread_cond_wait(&daq_status_cv,&daq_status_mutex);
	}
	pthread_mutex_unlock(&daq_status_mutex);
	while(!stop)
	{

		Event* from;
		Event* to=new Event(event_size*board_num);
		if(header_enable)
		{
			memcpy(to->buf(),&h,sizeof(h));
			to->addused(sizeof(h)/4);
		}
		for(int i=0;i<board_num;i++)
		{
			/*
			   if(stop==0)
			   {
			   try_times++;
			   usleep(300);     //in case a board slow
			   if(try_times>=200000)
			   {
			   __sync_fetch_and_add(&stop,1);
			   std::cout<<"Pack set stop 1"<<std::endl; 
			   }
			   }
			   else
			   {
			   break;
			   }
			   */
			/*
			   if(check_enable)
			   {
			   for(uint32_t j=0;j<event_size;j+=4)    
			   {
			   Exchange(p+j);
			   }

			   if(TIMES==0)
			   {   
			   L1id.push_back((uint16_t)(p[1]&0x0000ffff));
			   } 

			   Check(p,event_size,TIMES);
			   TIMES++;
			   }
			   */
			while(q_readout[i]->size()==0)
			{
				//DEBUG_INFO("pack000000");
				if(stop==1)  
				{ 
					DEBUG_INFO("Here");
					incom=true; break;
				}
			}
			if(incom) 
			{DEBUG_INFO("hh");
				break;}
			q_readout[i]->get(from);
			memcpy(to->buffer(),from->buf(),4*event_size);
			to->addused(event_size);
			delete from;
		}
		if(header_enable)
		{
			h.event_num++;
		}
		if(!incom)
		{
			if (display_thread)
			{
				if(first)
				{
					d_send= new DataSend();
					d_send->start();
					first=false;	
				}
				
				if(start_display && !stop_display && !stop)
				{
					if(rand()%10000<10)
					{
					Event* tobesend = new Event(to);
					q_send->put(tobesend);
					}
				}
			}
			if(save_enable)
			{
				q_pack->put(to);
			}
			else
			{
				delete to;
			}
		}
		else
		{
			delete to;
		}
	}
	DEBUG_INFO("block in datasend thread");
	delete d_send;
	std::cout<<"Pack thread exit"<<std::endl;
	return;//pthread_exit(NULL);
}
void Exchange(uint32_t *p)   //4words(128bits):1 2 3 4-->4 3 2 1 
{
	uint32_t temp;
	temp=p[0];
	p[0]=p[3];
	p[3]=temp;
	temp=p[1];
	p[1]=p[2];
	p[2]=temp;
}
void Check(uint32_t* p,uint32_t N,uint32_t TIMES)
{
	if((p[0]>>16)!=0x805a)
	{
		std::cout<<"error head"<<std::endl;
		//	WriteLog("error head",NULL);
	}
	if((p[N-1]&0x0000ffff)!=0x8069)
	{
		std::cout<<"error tail"<<std::endl;
		//	WriteLog("error tail",NULL);
	}
	if(TIMES!=0)
	{
		Check_l1id(p);
	}
	Check_data(p,N);
}
void Check_data(uint32_t* p,uint32_t N)
{
	uint16_t temp[2*(N-8)];
	for (uint32_t i=0;i<N-8;i++)
	{   
		temp[2*i+1]=(uint16_t)((p[i+4])&(0x0000ffff));
		temp[2*i]=(uint16_t)(p[i+4]>>16);
	}   

	for(uint32_t k=0;k<2*(N-8);k++)
	{   
		if((k+1)<(2*(N-8)))
		{
			if((temp[k]+1)!=temp[k+1])
			{
				//WriteLog("point not right",NULL);
			}
		}
	}   
}
void Check_l1id(uint32_t* p)
{
	uint16_t l1id=(p[1]&0x0000ffff);
	if((l1id-1)!=L1id.back())
	{   

		//	WriteLog("error l1id:now:",&l1id);
		//	WriteLog(" error l1id:last:",&(L1id.back()));
		//	WriteLog("\n",NULL);
		std::cout<<std::hex<<"error l1id"<<"last:"<<L1id.back()<<" now:"<<l1id<<std::endl;
	}   
	L1id.back()=l1id;
}
/*
   void Pack::destory()
   {

   }
   */
