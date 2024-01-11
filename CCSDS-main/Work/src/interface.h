#include "protocs/protocs.h"
#include <iostream>
#include <typeinfo>
#include <vector>


template <class Type>
class Interface
{
public:
	Interface(Type& networkPackage, size_t packageLen)
	{
		unsigned int prevFrameCounter = -1;
		using netPackageType = decltype(networkPackage[0]);
		for(size_t i = 0; i < packageLen; i++)
			{
				ParentVCDU* parentVCDU = new VCDU<netPackageType>(networkPackage[i]);
				if(parentVCDU->getToUlongVCID() != 63)
					this->vecVCDU.push_back(parentVCDU);
				
				unsigned int nowFrameCounter = parentVCDU->getToUlongFrameCounter();
				if(prevFrameCounter >= 0 && (nowFrameCounter-prevFrameCounter) != 1)
					throw std::runtime_error("violation of personnel order");
				prevFrameCounter = nowFrameCounter;
			}
	}
	
	~Interface()
	{
		for(auto p: this->vecVCDU)
			delete p;
		this->vecVCDU.clear();
	}
	
	size_t sizeVecVCDU() {return this->vecVCDU.size();}
private:
	std::vector<ParentVCDU*> vecVCDU;	
};
