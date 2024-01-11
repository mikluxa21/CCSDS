#include <interface.h>
#include <gtest/gtest.h>
#include <time.h>

TEST(Interface, ParseVCDU_CorruptedData)
{
	std::vector<std::vector<unsigned char>> vector;
	std::vector<unsigned char> vec{3, 3, 0, 0, 0, 3};
	for(size_t i = 0; i < 167776; i++)
		{
			vec[4] = i;
			vector.push_back(vec);
			if(vec[4] == 10)
				{
					vec[3]++;
					vec[4] = 0;
				}
			if(vec[3] == 255)
				{
					vec[2]++;
					vec[3] = 0;
				}	
		}
	EXPECT_THROW(Interface<std::vector<std::vector<unsigned char>>>(vector, vector.size()), std::runtime_error);
}

TEST(Interface, ParseVCDU_63)
{
	std::vector<std::vector<unsigned char>> vector;
	std::vector<unsigned char> vec{3, 63, 0, 0, 0, 3};
	for(size_t i = 0; i < 16777; i++)
		{
			vec[4] = i;
			vector.push_back(vec);
			if(vec[4] == 255)
				{
					vec[3]++;
					vec[4] = 0;
				}
			if(vec[3] == 255)
				{
					vec[2]++;
					vec[3] = 0;
				}	
		}
	EXPECT_TRUE(Interface<std::vector<std::vector<unsigned char>>>(vector, vector.size()).sizeVecVCDU() == 0);
}


TEST(CCSDS, OneFrame)
{
	unsigned int size = 1000;
	unsigned char size_1 = size/256;
	unsigned char size_2 = size%256;
	std::vector<unsigned char> vector = {230, 230, 230, 230, size_1, size_2};
	for(size_t i = 0; i < 8; i++)
	{
		unsigned char c = 234;
		vector.push_back(c);
	}
	for(size_t i = 0; i < size - 8; i++)
		{
			unsigned char c = 204;
			vector.push_back(c);
		}
	EXPECT_EQ(CCSDS<std::vector<unsigned char>>(vector).getCountUsedByte(), 1000 + 6);
	
}














