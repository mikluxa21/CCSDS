#include <bitset>
#include <string>
#include <vector>
#include <iostream>
#include <exception>

class ParentCCSDS
{
public:
	//TypeCCSDS getPayload() { return this->payload;}
	virtual size_t getCountUsedByte() { return 0;}
	
	virtual unsigned int getToUlongVersion() { return 0;}
	virtual unsigned int getToUlongType() { return 0;}
	virtual unsigned int getToUlongHelpersHeaderFlag() { return 0;}
	virtual unsigned int getToUlongAPID() { return 0;}
	virtual unsigned int getToUlongFragmentationFlag() { return 0;}
	virtual unsigned int getToUlongFrameCounter() { return 0;}
	virtual unsigned int getToUlongSize() { return 0;}
	
	std::bitset<64> getToBitsetTime() { return std::bitset<64>(0);}
};

template <class TypeCCSDS>
class CCSDS
{
public:
	CCSDS(TypeCCSDS& packageCCSDS)
	{
		std::string packageID = std::bitset<8>(packageCCSDS[0]).to_string() + std::bitset<8>(packageCCSDS[1]).to_string();
		this->version = std::bitset<3>(packageID[0] + packageID[1] + packageID[2]);
		this->type = std::bitset<1>(packageID[3]);
		this->helperHeaderFlag = std::bitset<1>(packageID[4]);
		for(size_t i = 6; i < 16; i++) this->APID[i-5] = std::bitset<1>(packageID[i])[0];
		std::string packageControl = std::bitset<8>(packageCCSDS[2]).to_string() + std::bitset<8>(packageCCSDS[3]).to_string();
		this->fragmentationFlag = std::bitset<2>(packageControl[0] + packageControl[1]);
		for(size_t i = 2; i < 16; i++) this->frameCounter[i-2] = std::bitset<1>(packageControl[i])[0];
		this->size = std::bitset<16>(std::bitset<8>(packageCCSDS[4]).to_string() + std::bitset<8>(packageCCSDS[5]).to_string());
		std::string strTime = "";
		for(size_t i = 6; i < 14; i++) strTime += std::bitset<8>(packageCCSDS[i]).to_string();
		this->time = std::bitset<64>(strTime);
		
		size_t size = this->size.to_ulong();
		for(size_t i = 14; i < size; i++)
			this->payload.push_back(packageCCSDS[i]);
		this->countUsedByte = 6 + size;
		
	}
	
	TypeCCSDS getPayload() { return this->payload;}
	size_t getCountUsedByte() { return this->countUsedByte;}
	
	unsigned int getToUlongVersion() { return this->version.to_ulong();}
	unsigned int getToUlongType() { return this->type.to_ulong();}
	unsigned int getToUlongHelpersHeaderFlag() { return this->helpersHeaderFlag.to_ulong();}
	unsigned int getToUlongAPID() { return this->APID.to_ulong();}
	unsigned int getToUlongFragmentationFlag() { return this->fragmentationFlag.to_ulong();}
	unsigned int getToUlongFrameCounter() { return this->frameCounter.to_ulong();}
	unsigned int getToUlongSize() { return this->size.to_ulong();}
	
	std::bitset<64> getToBitsetTime() { return this->time;}
private:
	TypeCCSDS payload;
	size_t countUsedByte;
	
	std::bitset<3> version;
	std::bitset<1> type;
	std::bitset<1> helperHeaderFlag;
	std::bitset<11> APID;
	std::bitset<2> fragmentationFlag;
	std::bitset<14> frameCounter;
	std::bitset<16> size;
	
	std::bitset<64> time;  //Ошибка в постановке задачи
};


class ParentVCDU
{
public:
	virtual unsigned int getToUlongVersion(){ return 0;}
	virtual unsigned int getToUlongObjID(){ return 0;}
	virtual unsigned int getToUlongVCID(){ return 0;}
	virtual unsigned int getToUlongFrameCounter(){ return 0;}
	virtual unsigned int getToUlongRepetitionFlag(){ return 0;}
	virtual unsigned int getToUlongReserve(){ return 0;}
};


template <class TypeVCDU>
class VCDU: public ParentVCDU
{
public:
	VCDU(TypeVCDU& packageVCDU)
	{	
		this->reserve = std::bitset<7>(packageVCDU[5]);
		this->repetitionFlag[0] = std::bitset<8>(packageVCDU[5])[0];
		this->frameCounter = std::bitset<24>(std::bitset<8>(packageVCDU[2]).to_string() + std::bitset<8>(packageVCDU[3]).to_string() + std::bitset<8>(packageVCDU[4]).to_string());
		std::string data = std::bitset<8>(packageVCDU[0]).to_string() + std::bitset<8>(packageVCDU[1]).to_string();
		for(size_t i = 0; i < 2; i++) this->version[i] = std::bitset<1>(data[i])[0];
		for(size_t i = 0; i < 8; i++) this->ObjID[i] = std::bitset<1>(data[2 + i])[0];
		for(size_t i = 0; i < 6; i++) this->VCID[i] = std::bitset<1>(data[10 + i])[0];
		
		this->offsetBit = std::bitset<16>(std::bitset<8>(packageVCDU[6]).to_string() + std::bitset<8>(packageVCDU[7]).to_string()).to_string());
		
		size_t currentOffset = 6 + 2 + this->offsetBit.to_uint();
		TypeVCDU::iterator iterBegin = packageVCDU.begin();
		TypeVCDU::iterator iterEnd = packageVCDU.end();
		unsigned int fragmentationFlag = 0
		do
		{
			iterBegin += currentOffset;
			ParentCCSDS* parentVCCSDS = new CCSDS<TypeVCDU>(TypeVCDU(iterBegin, iterEnd));
			vecCCSDS.push_back(parentVCCSDS);
			fragmentationFlag = parentVCCSDS->getToUlongFragmentationFlag();
			currentOffset += parentVCCSDS->getCountUsedByte();
		}
		while(fragmentationFlag < 2) //01->start | 00->midle | 10->end | 11->one
		
	}
	~CCSDS()
	{
		for(auto p: this->vecCCSDS)
			delete p;
		this->vecCCSDS.clear();
	}
	
	unsigned int getToUlongVersion(){ return this->version.to_ulong();}
	unsigned int getToUlongObjID(){ return this->ObjID.to_ulong();}
	unsigned int getToUlongVCID(){ return this->VCID.to_ulong();}
	unsigned int getToUlongFrameCounter(){ return this->frameCounter.to_ulong();}
	unsigned int getToUlongRepetitionFlag(){ return this->repetitionFlag.to_ulong();}
	unsigned int getToUlongReserve(){ return this->reserve.to_ulong();}
	
private:
	std::vector<ParentCCSDS*> vecCCSDS;	
	
	std::bitset<2> version;
	std::bitset<8> ObjID;
	std::bitset<6> VCID;
	std::bitset<24> frameCounter;
	std::bitset<1> repetitionFlag;
	std::bitset<7> reserve;
	
	std::bitset<16> offsetBit;
	size_t resultOffset;
};





