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
	const unsigned int time_begin;
	const unsigned int duration;

public:
	//muteConverter(Thread& thread, unsigned int time_begin, unsigned int duration) : thread(thread), time_begin(time_begin), duration(duration) {}
	muteConverter(std::vector<std::string> threadFiles, std::vector<unsigned int> parameters) : threadFile(threadFiles[0]), time_begin(parameters[0]), duration(parameters[1]) {}
	~muteConverter() = default;

	Thread convert() override;
};

