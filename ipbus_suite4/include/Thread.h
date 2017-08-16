#ifndef __THREAD_H__
#define __THREAD_H__
#include <pthread.h>
#include <iostream>
//#include <sys/types.h>
class Thread
{
    private:
	pthread_t pid;
    private:
	static void* start_thread(void* args);
    public:
	Thread();
	//~Thread();
	pthread_t getpid();
	int start();
	virtual void run()=0;
	void join();
};
#endif
