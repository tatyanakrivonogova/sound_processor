#pragma once
#include "Converter.h"
#include "Thread.h"

class slowConverter : public Converter
{
private:
	std::string threadFile;
	unsigned int time_begin = 0;
	unsigned int duration;
	unsigned int ratio;
	std::shared_ptr<std::string> outputFile = nullptr;

public:
	slowConverter() = default;
	slowConverter(std::vector<std::string>, std::vector<unsigned int>, std::shared_ptr<std::string>);
	~slowConverter() = default;

	Thread convert() override;
	void whatAreYouDoing(FILE*) override;
};

