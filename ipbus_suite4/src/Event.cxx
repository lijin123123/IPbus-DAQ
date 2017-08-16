#include "Event.h"
#include <cstring>
#include "gva.h"
#define MAX_EVENT_SIZE 1024*6
Buffer Event::buffer_pool(MAX_EVENT_SIZE,3000); 
Event::Event(uint32_t max_size)
{
	m_max_size = max_size;
	m_buffer=(uint32_t*)buffer_pool.malloc();
        memset(m_buffer,2,max_size*4);
        m_now_buffer=m_buffer;
}
Event::Event(Event* e)
{
	//DEBUG_INFO("in");
	m_max_size=e->m_max_size;
	m_buffer=(uint32_t*)buffer_pool.malloc();
        //DEBUG_INFO("mid");
	memcpy(m_buffer,e->buf(),m_max_size*4);
	m_now_buffer=m_buffer;
        //DEBUG_INFO("out");
}
Event::~Event()
{
	buffer_pool.free(m_buffer);
}

