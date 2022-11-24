#pragma once
#include <vector>
#include "Converter.h"
#include "Thread.h"

class mixConverter : public Converter
{
private:
	Thread thread1;
	Thread thread2;
	const unsigned int time_begin;
	//const unsigned int duration;

public:
	//muteConverter(Thread& thread, unsigned int time_begin, unsigned int duration) : thread(thread), time_begin(time_begin), duration(duration) {}
	mixConverter(std::vector<Thread> threads, std::vector<unsigned int> time_args) : thread1(threads[0]), thread2(threads[1]), time_begin(time_args[0]) {}
	~mixConverter() = default;

	Thread convert() override;
};

