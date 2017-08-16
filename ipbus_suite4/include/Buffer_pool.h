#pragma once
#include <stack>
#include <stdint.h>
#include <cstddef>
#include "pthread.h"
class Buffer
{
    private:
	uint32_t* buf_start;
	std::stack<uint32_t*> BufPool;
	pthread_mutex_t mutex;
    public:
	Buffer(size_t eachsize,size_t count);
	~Buffer();
	uint32_t* GetBufStart();
	uint32_t GetBufSize();
	//bool GetBufPool(std::queue<uint32_t*> &);
	void* malloc();
	void free(void* p);
	
};
