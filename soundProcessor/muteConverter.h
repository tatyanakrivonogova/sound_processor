#pragma once
#include <vector>
#include <string>
#include "Converter.h"
#include "Thread.h"

class muteConverter : public Converter
{
private:
	std::string threadFile;
	unsigned int time_begin = 0;
	unsigned int duration;

public:
	muteConverter() = default;
	muteConverter(std::vector<std::string>, std::vector<unsigned int>);
	~muteConverter() = default;

	Thread convert() override;
	void whatAreYouDoing(FILE*) override;
};

