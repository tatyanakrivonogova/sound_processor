#pragma once
#include "Stream.h"
#include <cstdio>

class Converter
{
public:
	virtual void whatAreYouDoing(FILE*) = 0;
	virtual Stream convert(std::vector<std::string>, std::vector<double>, std::shared_ptr<std::string> outputFile = nullptr) = 0;
	virtual ~Converter() = default;

};