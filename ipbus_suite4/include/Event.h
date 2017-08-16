#pragma once
#include "Buffer_pool.h"
#pragma pack(1)
struct header
{
	uint32_t tag;
	uint32_t event_num;
        uint32_t length;
};
#pragma pack()
class Event
{
	private:
		static Buffer buffer_pool;
		uint32_t* m_buffer;
                uint32_t* m_now_buffer;
		uint32_t m_used_size;
		uint32_t m_max_size;
			
                
	public:
		Event(uint32_t max_size);
                Event(Event*);
		~Event();
                inline uint32_t max_size()	{return m_max_size;}
                inline uint32_t* buf()      {return m_buffer;}          
		inline uint32_t* buffer()	{return m_now_buffer;}
                inline uint32_t* addused(uint32_t size)    {return m_now_buffer=m_now_buffer+size;}
                inline uint32_t* mul_header(uint32_t size) {return m_buffer+size;}   //used for copy construct
};
