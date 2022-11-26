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
	std::shared_ptr<std::string> outputFile = nullptr;

public:
	muteConverter() = default;
	muteConverter(std::vector<std::string>, std::vector<unsigned int>, std::shared_ptr<std::string>);
	~muteConverter() = default;

	Thread convert() override;
	void whatAreYouDoing(FILE*) override;
};

