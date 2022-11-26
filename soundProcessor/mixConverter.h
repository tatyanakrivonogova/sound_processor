#pragma once
#include <vector>
#include <cstdio>
#include "Converter.h"
#include "Thread.h"

class mixConverter : public Converter
{
private:
	std::string threadFile1;
	std::string threadFile2;
	unsigned int time_begin = 0;
	std::shared_ptr<std::string> outputFile = nullptr;

public:
	mixConverter() = default;
	mixConverter(std::vector<std::string>, std::vector<unsigned int>, std::shared_ptr<std::string>);
	~mixConverter() = default;

	Thread convert() override;
	void whatAreYouDoing(FILE*) override;
};

