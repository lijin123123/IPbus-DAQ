#pragma once
#include "LThread.h"
#include "RThread.h"
#include "LogSend.h"
class Control
{
    private:
	RThread* run;
	LThread* listen;
        LogSend* log;
    public:
	void Initial();
	void start();
	void clear();
};
