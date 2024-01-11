#include <interface.h>
#include <gtest/gtest.h>
#include <time.h>
#include "Help/Test_Helpers.h"

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
	std::vector<unsigned char> header = createCCSDSHeader(7, 1, 1, 2047, 3, 16383, 6553);
	std::vector<unsigned char> payload = createCCSDSBody(6553);
	header.insert(header.end(), std::make_move_iterator(payload.begin()), std::make_move_iterator(payload.end()));
	CCSDS<std::vector<unsigned char>> ccsds(header);
	
	EXPECT_EQ(ccsds.getPayload(), payload);
	EXPECT_EQ(ccsds.getCountUsedByte(), header.size());
	EXPECT_EQ(ccsds.getToUlongVersion(), 7);
	EXPECT_EQ(ccsds.getToUlongType(), 1);
	EXPECT_EQ(ccsds.getToUlongHelpersHeaderFlag(), 1);
	EXPECT_EQ(ccsds.getToUlongAPID(), 2047);
	EXPECT_EQ(ccsds.getToUlongFragmentationFlag(), 3);
	EXPECT_EQ(ccsds.getToUlongFrameCounter(), 16383);
	EXPECT_EQ(ccsds.getToUlongSize(), 6553);
	EXPECT_TRUE(ccsds.getToBitsetTime().all());
}

TEST(CCSDS, FewFrame)
{
		std::vector<unsigned char> header = createCCSDSHeader(7, 1, 1, 2047, 3, 16383, 6553);
	std::vector<unsigned char> payload = createCCSDSBody(6553);
	header.insert(header.end(), std::make_move_iterator(payload.begin()), std::make_move_iterator(payload.end()));
	
	for(size_t i = 0; i < 2; i++)
		{
			for(auto i: header)
				header.push_back(i);
		}
	size_t svipe = 0;
	for(size_t i = 0; i < 2; i++)
	{
		std::vector<unsigned char> v(header.begin() + svipe, header.end());
		std::cerr << ">>>>>>>>>" << v.size()/(6553 + 14) << std::endl;
		CCSDS<std::vector<unsigned char>> ccsds(v);
		EXPECT_EQ(ccsds.getPayload(), payload);
		EXPECT_EQ(ccsds.getCountUsedByte(), header.size());
		EXPECT_EQ(ccsds.getToUlongVersion(), 7);
		EXPECT_EQ(ccsds.getToUlongType(), 1);
		EXPECT_EQ(ccsds.getToUlongHelpersHeaderFlag(), 1);
		EXPECT_EQ(ccsds.getToUlongAPID(), 2047);
		EXPECT_EQ(ccsds.getToUlongFragmentationFlag(), 3);
		EXPECT_EQ(ccsds.getToUlongFrameCounter(), 16383);
		EXPECT_EQ(ccsds.getToUlongSize(), 6553);
		EXPECT_TRUE(ccsds.getToBitsetTime().all());
		svipe += ccsds.getCountUsedByte();
		
	}
}

TEST(Test_Helpers, CreateVCDUHeader_1)
{
	for(auto i: createVCDUHeader(3, 255, 63, 16777215, 1, 127))
		EXPECT_EQ((int) i, 255);
}

TEST(Test_Helpers, CreateVCDUHeader_2)
{
	std::vector<unsigned char> vec = createVCDUHeader(1, 255, 60, 4160459, 1, 126);
	EXPECT_EQ((int) vec[0], 127);
	EXPECT_EQ((int) vec[1], 252);
	EXPECT_EQ((int) vec[2], 203);
	EXPECT_EQ((int) vec[3], 123);
	EXPECT_EQ((int) vec[4], 63);
	EXPECT_EQ((int) vec[5], 254);
	
}

TEST(Test_Helpers, CreateCCSDSHeader_1)
{
	for(auto i: createCCSDSHeader(7, 1, 1, 2047, 3, 16383, 65535))
		EXPECT_EQ((int) i, 255);
}

TEST(Test_Helpers, CreateCCSDSHeader_2)
{
	std::vector<unsigned char> vec = createCCSDSHeader(0, 1, 1, 2044, 0, 16380, 65534);
	EXPECT_EQ((int) vec[0], 31);
	EXPECT_EQ((int) vec[1], 252);
	EXPECT_EQ((int) vec[2], 63);
	EXPECT_EQ((int) vec[3], 252);
	EXPECT_EQ((int) vec[4], 255);
	EXPECT_EQ((int) vec[5], 254);
	
}


TEST(Test_Helpers, createCCSDSBody)
{
	size_t size = 123456;
	EXPECT_EQ(createCCSDSBody(size).size(), size);
}











