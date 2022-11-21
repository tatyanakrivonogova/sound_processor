#pragma once
#include "Buffer.h"

class readBuffer : public Buffer
{
public:
	readBuffer(size_t BUFF_SIZE, FILE* file, size_t position) : Buffer(BUFF_SIZE, file, position) {}
	//const short* readData(size_t count);
	const short operator[](size_t pos);
};

