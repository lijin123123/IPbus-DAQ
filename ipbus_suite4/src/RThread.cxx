#include "RThread.h"
#include "gva.h"
#include "Config.h"
#include "Base.h"
#include "ReadOut.h"
#include <sstream>
void RThread::run()
{
    Config C;
    C.GetConfInfo();

    sem_wait(&l_r_sem);
    //std::cout<<"config wait over"<<std::endl;
    pthread_mutex_lock(&mutex);
    std::vector<std::string> devicename;
    std::string name="MyDevice";
    std::string num;
    for(int i=0;i<board_num;i++)
    {
	std::stringstream ss;
        ss<<i;
        ss>>num;
        devicename.push_back(name+num);
    }
   // devicename.push_back("MyDevice0");
    
    Base B("file://connections.xml",devicename);
    //C.WriteRead();
    done=1;
    if(done==1)
    {
	pthread_cond_signal(&cv);
    }
    pthread_mutex_unlock(&mutex);
    printf("!config done\n");

    sem_wait(&l_r_sem);
    pthread_mutex_lock(&mutex);
    ReadOut* R=ReadOut::instance();
    //ReadOut* R=new ReadOut();
    //Pack* P=new Pack();
    //SaveToDisk* S=new SaveToDisk();
    R->start();
    //P->start();
    //if(save_enable)
    //{
   // S->start();
   // }
    done=1;
    printf("!start done!\n");
    if(done=1)
    {
	pthread_cond_signal(&cv);
    }
    pthread_mutex_unlock(&mutex);

    sem_wait(&l_r_sem);
    pthread_mutex_lock(&mutex);
    delete R;
    //delete P;
    //delete S;
    //R.join();
    //P.join();
    //S.join();
    done=1;
    printf("!stop done!\n");
    if(done==1)
    {
	pthread_cond_signal(&cv);
    }
    pthread_mutex_unlock(&mutex);
    return ;//pthread_exit(NULL);
}
