#ifndef __LTHREAD_H__
#define __LTHREAD_H__
#include "Thread.h"
enum CMDTYPE {CONNECT,CONFIG,START,STOP,DISCONNECT,SAVE,DISPLAY,START_DISPLAY,STOP_DISPLAY};
class LThread:public Thread
{
    public:
	void run();
	void prepare_socket();
        void do_lock();
	//void in_cmd(char* cmd);
	LThread(){}
	~LThread()
	{
	    join();
	}
    private:
	int accept_sock;
        int sock;
};
#endif
