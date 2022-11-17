#pragma once
#include "Converter.h"
#include "Thread.h"

class muteConverter : Converter
{
private:
	Thread& thread;
	const unsigned int time_begin;
	const unsigned int duration;

public:
	muteConverter(Thread& thread, unsigned int time_begin, unsigned int duration) : thread(thread), time_begin(time_begin), duration(duration) {}
	~muteConverter() = default;

	Thread convert() override;
};

