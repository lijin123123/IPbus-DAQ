#include <map>
#include <utility>
#include <string>
#include "gva.h"
class Config
{
    private:
	std::string NodeName;
	//HwInterface* HW[board_num];
	std::map<std::string,std::pair<uint32_t,uint32_t> > ConfInfo;
    public:
	//Config(Base& B);
	void Read(uint32_t N);
	void Write(uint32_t N,uint32_t num);
	void WriteRead();
	void GetConfInfo();
};
