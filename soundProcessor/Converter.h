#pragma once
#include "Thread.h"

class Converter
{
public:
	virtual Thread convert() = 0;
	virtual ~Converter() = default;

};