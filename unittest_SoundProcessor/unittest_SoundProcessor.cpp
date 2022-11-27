#include "gtest\gtest.h"
#include "soundProcessor.h"

TEST(soundProcessorTests, test1) {
	Thread thread(std::make_shared<std::string>("test"));
	EXPECT_TRUE(2 + 2 == 4);
}