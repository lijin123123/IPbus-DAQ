#include "Buffer_pool.h"
#include "gva.h"
#include <cstdio>
Buffer::Buffer(size_t eachsize,size_t count)
{
    pthread_mutex_init(&mutex,NULL);
    buf_start=new uint32_t[eachsize*count];    //words
    for(size_t i=0;i<count;i++)
    {
	BufPool.push(buf_start+eachsize*i);
    }
}
Buffer::~Buffer()
{
    delete [] buf_start;
}
uint32_t* Buffer::GetBufStart()
{
    return buf_start;
}
uint32_t Buffer::GetBufSize()
{
   //DEBUG_INFO(BufPool.size());
   return BufPool.size();
}
void* Buffer::malloc()
{
    void* p;
    pthread_mutex_lock(&mutex);
    p=BufPool.top();
    BufPool.pop();
    pthread_mutex_unlock(&mutex);
    return p;
}
void Buffer::free(void* p)
{
    pthread_mutex_lock(&mutex);
    BufPool.push((uint32_t*)p);
    pthread_mutex_unlock(&mutex);
}
/*
bool Buffer::GetBufPool(std::queue<uint32_t*>& temp)
{
    temp=BufPool;
}
*/
