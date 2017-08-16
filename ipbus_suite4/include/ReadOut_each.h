#pragma once
#include "gva.h"
#include "Thread.h"
//extern ProtectedQueue<Event*>* get_read_Q(int i); 
class ReadOut_each:public Thread
{
    private:
	int board_n;
        size_t expected_l1id;
        size_t actual_l1id;
        uint32_t* p; 
        bool first_packet;
        bool check_enable;
    public:
	ReadOut_each(int n);
	~ReadOut_each();
	int get_board_n();
	void run();
        inline bool check_header();
        inline bool check_length();
        inline bool check_tailer();
        inline bool check_trigger();
};
