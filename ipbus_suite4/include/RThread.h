#include "Thread.h"
class RThread:public Thread
{
    public:
	void run();
	RThread(){}
	~RThread()
	{
	    join();
	}
};
