#include "gva.h"
#include "Thread.h"
//extern ProtectedQueue<Event*>* get_pack_Q();
class SaveToDisk:public Thread
{
    public:
	void run();
	SaveToDisk();
	~SaveToDisk();
};
