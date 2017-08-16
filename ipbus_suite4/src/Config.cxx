#include <cstdio>
#include "Config.h"
/*
   Config::Config(Base& B)
   {
   for(int i=0;i<board_num;i++)
   {
   HW[i]=B.HW[i];
   }
   }
   */
void Config::GetConfInfo()
{
    FILE* fp; 
    if((fp=fopen("config.txt","r"))==NULL)
    {
	//log
	exit(0);
    }
    std::pair<uint32_t,uint32_t> val(0,0);
    char key[20];
    //get board_num
    fscanf(fp,"%s",key);
    if(strcmp(key,"board_num")==0)
    {
	fscanf(fp,"%x",&board_num);
    }
    //get bufsize
    fscanf(fp,"%s",key);
    if(strcmp(key,"bufsize")==0)
    {
	fscanf(fp,"%x",&bufsize);
    }
    //get event_size
    fscanf(fp,"%s",key);
    if(strcmp(key,"event_size")==0)
    {
	fscanf(fp,"%x",&event_size);
    }
    while((fscanf(fp,"%s",key))!=EOF)
    {
	fscanf(fp,"%d",&(val.first));
	fscanf(fp,"%x",&(val.second));
	ConfInfo.insert(std::map<std::string,std::pair<uint32_t,uint32_t> >::value_type(key,val));
    }
    /*
       for(std::map<std::string,std::pair<uint32_t,uint32_t> >::iterator map_it=ConfInfo.begin();map_it!=ConfInfo.end();map_it++)
       {
       std::cout<<map_it->first<<std::endl;
       std::cout<<(map_it->second).first<<std::endl;
       std::cout<<(map_it->second).second<<std::endl;
       }
       */
}
void Config::Read(uint32_t N)
{
    for(int j=0;j<board_num;j++)
    {
	ValVector<uint32_t> mem;
	mem=HW[j]->getNode(NodeName).readBlock(N);
	try 
	{   
	    HW[j]->dispatch();
	    for(uint32_t i=0;i<N;i++)
	    {
		std::cout<<mem[i]<<std::endl;
	    }

	}   
	catch(std::exception &e) 
	{   
	    //WriteLog()
	}
    }
}
void Config::Write(uint32_t N,uint32_t num)
{
    for(int j=0;j<board_num;j++)
    {
	std::vector<uint32_t> xx; 
	for(uint32_t i=0;i<N;i++)
	{   
	    xx.push_back(num);
	}   
	HW[j]->getNode(NodeName).writeBlock(xx);
	try 
	{   
	    HW[j]->dispatch();
	}   
	catch(std::exception& e)
	{   

	}
    }

}
void Config::WriteRead()
{
    for(std::map<std::string,std::pair<uint32_t,uint32_t> >::iterator map_it=ConfInfo.begin();map_it!=ConfInfo.end();map_it++)
    {
    /*
	std::cout<<map_it->first<<std::endl;
	std::cout<<(map_it->second).first<<std::endl;
	std::cout<<(map_it->second).second<<std::endl;
	*/
	NodeName=map_it->first;
	uint32_t N=(map_it->second).first;
	uint32_t num=(map_it->second).second;
	Write(N,num);
	Read(N);
    }
}

