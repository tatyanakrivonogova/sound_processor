#pragma once
#include "Thread.h"
#include <cstdio>

class Converter
{
public:
	virtual void whatAreYouDoing(FILE*) = 0;
	virtual Thread convert(std::vector<std::string>, std::vector<unsigned int>, std::shared_ptr<std::string> outputFile = nullptr) = 0;
	virtual ~Converter() = default;

};