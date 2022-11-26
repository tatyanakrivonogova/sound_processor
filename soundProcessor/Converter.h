#pragma once
#include "Thread.h"
#include <cstdio>

class Converter
{
public:
	virtual void whatAreYouDoing(FILE*) = 0;
	virtual Thread convert() = 0;
	virtual ~Converter() = default;

};