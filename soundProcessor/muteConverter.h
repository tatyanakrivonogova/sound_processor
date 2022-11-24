#pragma once
#include <vector>
#include "Converter.h"
#include "Thread.h"

class muteConverter : public Converter
{
private:
	Thread thread;
	const unsigned int time_begin;
	const unsigned int duration;

public:
	//muteConverter(Thread& thread, unsigned int time_begin, unsigned int duration) : thread(thread), time_begin(time_begin), duration(duration) {}
	muteConverter(std::vector<Thread> threads, std::vector<unsigned int> time_args) : thread(threads[0]), time_begin(time_args[0]), duration(time_args[1]) {}
	~muteConverter() = default;

	Thread convert() override;
};

