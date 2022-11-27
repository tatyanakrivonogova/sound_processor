#pragma once
#include <vector>
#include <cstdio>
#include "Converter.h"
#include "Thread.h"

class mixConverter : public Converter
{
private:

public:
	mixConverter() = default;
	~mixConverter() = default;

	Thread convert(std::vector<std::string>, std::vector<unsigned int>, std::shared_ptr<std::string>) override;
	void whatAreYouDoing(FILE*) override;
};

