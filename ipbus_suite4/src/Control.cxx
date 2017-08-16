#include "Control.h"
#include "gva.h"
#include <iostream>
void Control::Initial()
{
    /*
       for(int i=0;i<board_num;i++)
       {
       while(!tempRFifo[i].empty())
       {
       tempRFifo[i].pop();
       }
       }
       while(!tempPFifo.empty())
       {
       tempPFifo.pop();
       }
       */
    pthread_cond_init(&cv,NULL);
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&daq_status_cv,NULL);
    pthread_mutex_init(&daq_status_mutex,NULL);
    
    sem_init(&l_r_sem,0,0);
    done=false;
    stop=false;
}
void Control::start()
{
    q_log = new ProtectedQueue<loginfo>(1000);
    listen=new LThread();
    run=new RThread();
    log=new LogSend(); 
    listen->start();
    std::cout<<"!listern thread start!"<<std::endl;
    run->start();
    std::cout<<"!run thread start!"<<std::endl;
    log->start();
    std::cout<<"!log thread start!"<<std::endl;
    delete run;
    std::cout<<"!run thread stop!"<<std::endl;
    delete listen;
    std::cout<<"!listen thread stop!"<<std::endl;
    delete log;
    std::cout<<"!log thread stop!"<<std::endl;
}
void Control::clear()
{
    sem_destroy(&l_r_sem);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cv);
    pthread_mutex_destroy(&daq_status_mutex);
    pthread_cond_destroy(&daq_status_cv);


    
    done=0;
    stop=0;
}
