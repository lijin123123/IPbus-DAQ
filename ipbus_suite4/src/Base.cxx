#include "Base.h"
#include <iostream>
Base::Base(std::string confilename,std::vector<std::string> devicename)
{
    ConFileName=confilename;
    DeviceName.assign(devicename.begin(),devicename.end());
    
    for(int i=0;i<board_num;i++)
    {
	DeviceName[i]=devicename[i];
    }
/*
    std::vector<std::string>::iterator it;
    for(it=DeviceName.begin();it!=DeviceName.end();++it)
    {
	std::cout<<*it<<std::endl;
    }
    */
    ConnectionManager manager(ConFileName);
    for(int i=0;i<board_num;i++)
    {
	HwInterface* hw=new HwInterface(manager.getDevice(DeviceName[i]));
	HW.push_back(hw);
    }
}

