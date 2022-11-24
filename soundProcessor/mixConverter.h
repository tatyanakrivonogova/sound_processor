#pragma once
#include <vector>
#include "Converter.h"
#include "Thread.h"

class mixConverter : public Converter
{
private:
	std::string threadFile1;
	std::string threadFile2;
	const unsigned int time_begin;
	//const unsigned int duration;

public:
	//muteConverter(Thread& thread, unsigned int time_begin, unsigned int duration) : thread(thread), time_begin(time_begin), duration(duration) {}
	mixConverter(std::vector<std::string> threadFiles, std::vector<unsigned int> time_args) : threadFile1(threadFiles[0]), threadFile2(threadFiles[1]), time_begin(time_args[0]) {}
	~mixConverter() = default;

	Thread convert() override;
};

