#pragma once
#include <semaphore.h>
#include <stdint.h>
#include <vector>
#include <queue>
#include "boost/filesystem.hpp"
#include "uhal/uhal.hpp"
#include "pthread.h"
#include "protected_queue.h"
#include "Event.h"
#define DEBUG
#ifdef DEBUG
	#define DEBUG_INFO(info) std::cout<<__TIME__<<" | "<<__FILE__<<" | "<<__LINE__<<" | "<<info<<std::endl;
#else
	#define DEBUG_INFO(info) std::cout<<info<<std::endl;
#endif
using namespace uhal;
extern uint32_t board_num;
extern uint32_t event_size;//words(4bytes)
extern uint32_t bufsize;
extern std::vector<HwInterface*> HW;
extern std::vector<uint16_t> L1id;
extern sem_t l_r_sem;
extern pthread_cond_t cv;
extern pthread_mutex_t mutex;



extern bool header_enable;
extern volatile int display_thread;
extern char msg[10];
extern bool done;
extern bool save_enable;
extern volatile int stop;
extern volatile int stop_display;
extern volatile int start_display;

extern pthread_mutex_t daq_status_mutex;
extern pthread_cond_t daq_status_cv;
enum STATUS{STARTING,RUNNING,STOPPING};
enum LEVEL{INFO,WARNNING,ERROR,FATAL};
extern STATUS status;

struct loginfo
{
	char log[100];
	time_t logtime;
	LEVEL level;
};


extern std::vector<ProtectedQueue<Event*>*> q_readout;
extern ProtectedQueue<Event*>* q_pack;
extern ProtectedQueue<Event*>* q_send;
extern ProtectedQueue<loginfo> * q_log;

extern void log_create(std::string s,LEVEL l,time_t t);
