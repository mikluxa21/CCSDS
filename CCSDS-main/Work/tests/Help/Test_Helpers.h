#include <bitset>
#include <vector>
#include <string>
#include <iostream>

std::vector<unsigned char> createVCDUHeader(unsigned char a1, unsigned char a2, unsigned char a3, unsigned int a4, unsigned int a5, unsigned int a6)
{
	std::vector<unsigned char> vector;
	std::string bitStr = "";
	bitStr += std::bitset<2>(a1).to_string();
	bitStr += std::bitset<8>(a2).to_string();
	bitStr += std::bitset<6>(a3).to_string();
	for(size_t i = 0; i < 3; i++)
		{
			bitStr += std::bitset<8>(a4%256).to_string();
			a4 = a4 >> 8;
		}
	bitStr += std::bitset<1>(a5).to_string();
	bitStr += std::bitset<7>(a6).to_string();
	for(size_t i = 0; i < 6; i++)
		{
			std::string byteStr = "";
			for(size_t j = 0; j < 8; j++)
				{
					byteStr += bitStr[8*i + j];
				}
			vector.push_back(std::bitset<8>(byteStr).to_ulong());
		}
	return vector;
}


std::vector<unsigned char> createCCSDSHeader(unsigned char a1, unsigned char a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6, unsigned int a7)
{
	std::vector<unsigned char> vector;
	std::string bitStr = "";
	bitStr += std::bitset<3>(a1).to_string();
	bitStr += std::bitset<1>(a2).to_string();
	bitStr += std::bitset<1>(a3).to_string();
	
	bitStr += std::bitset<3>(a4/256).to_string();
	bitStr += std::bitset<8>(a4%256).to_string();
	
	bitStr += std::bitset<2>(a5).to_string();
	
	bitStr += std::bitset<6>(a6/256).to_string();
	bitStr += std::bitset<8>(a6%256).to_string();
	
	bitStr += std::bitset<8>(a7/256).to_string();
	bitStr += std::bitset<8>(a7%256).to_string();
	for(size_t i = 0; i < 64; i++) bitStr += "1";
	for(size_t i = 0; i < 14; i++)
	{
		std::string byteStr = "";
		for(size_t j = 0; j < 8; j++)
			{
				byteStr += bitStr[8*i + j];
			}
		vector.push_back(std::bitset<8>(byteStr).to_ulong());
	}
	return vector;
}



std::vector<unsigned char> createCCSDSBody(size_t size)
{
	std::vector<unsigned char> vector;
	for(size_t i = 0; i < size; i++)
		vector.push_back(std::rand()%256);
	return vector;
}





