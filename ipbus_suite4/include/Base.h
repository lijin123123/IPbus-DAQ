#pragma once
#include <vector>
#include <string>
#include <stdint.h>
#include "gva.h"
using namespace uhal;
class Base
{
    private:
	std::string ConFileName;
	std::vector<std::string> DeviceName;
    public:
	//HwInterface* HW[board_num];
	Base(std::string confilename,std::vector<std::string> devicename);
	//friend class ReadOut;
	//friend class Config;
	//virtual bool Read(uint32_t N)=0;
	//virtual bool Write(uint32_t N,uint32_t num)=0;
	//void WriteLog(std::string szlog,uint16_t*  num);
	/*
	void Check(uint32_t* p,uint32_t N,uint32_t TIMES);
	bool Check_l1id(uint32_t* p,uint32_t N);
	bool Check_data(uint32_t* p,uint32_t N);
	bool Save(uint32_t* p,char* filename,uint32_t N);
	*/
	

};
