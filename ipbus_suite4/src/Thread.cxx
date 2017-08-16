#include "Thread.h"
Thread::Thread()
{
}
int Thread::start()
{
    if(pthread_create(&pid,NULL,start_thread,(void*)this)!=0)
    {
	std::cout<<"create thread error"<<std::endl;
	return -1;
    }
    return 0;
}
void* Thread::start_thread(void* args)
{
    Thread* ptr=(Thread*)args;
    ptr->run();
}
void Thread::join()
{
    pthread_join(pid,NULL);
}
/*
Thread::~Thread()
{
    join();
}
*/
pthread_t Thread::getpid()
{
    return pid;
}


