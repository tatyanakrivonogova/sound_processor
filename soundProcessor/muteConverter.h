#pragma once
#include <vector>
#include <string>
#include "Converter.h"
#include "Thread.h"

class muteConverter : public Converter
{
private:
	//Thread thread;
	std::string threadFile;
	unsigned int time_begin = 0;
	unsigned int duration;

public:
	//muteConverter(Thread& thread, unsigned int time_begin, unsigned int duration) : thread(thread), time_begin(time_begin), duration(duration) {}
	muteConverter(std::vector<std::string>, std::vector<unsigned int>);
	~muteConverter() = default;

	Thread convert() override;
};

