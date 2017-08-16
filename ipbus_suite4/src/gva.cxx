#include "gva.h"

std::vector<HwInterface*> HW;
std::vector<uint16_t> L1id;
uint32_t board_num=1;
uint32_t event_size=0;
uint32_t bufsize=0;
char msg[10];
pthread_cond_t cv;
pthread_mutex_t mutex;
bool done=0;                   //0:not done 1:done
bool save_enable=0;
volatile int stop=0;           //1:stop
volatile int stop_display=0;
volatile int start_display =0;
volatile int display_thread = 0;
sem_t l_r_sem;

STATUS status;
pthread_cond_t daq_status_cv;
pthread_mutex_t daq_status_mutex;


std::vector<ProtectedQueue<Event*>*> q_readout;
ProtectedQueue<Event*>* q_pack;
ProtectedQueue<Event*>* q_send;
ProtectedQueue<loginfo>* q_log;


bool header_enable =true;






