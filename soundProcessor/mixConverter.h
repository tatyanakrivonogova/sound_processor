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

public:
	mixConverter() = default;
	mixConverter(std::vector<std::string>, std::vector<unsigned int>);
	~mixConverter() = default;

	Thread convert() override;
	void whatAreYouDoing(FILE*) override;
};

