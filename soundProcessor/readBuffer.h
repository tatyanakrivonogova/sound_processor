#pragma once
#include "Buffer.h"

class readBuffer : public Buffer
{
private:
	size_t fullness = 0;
public:
	readBuffer(size_t BUFF_SIZE, FILE* file, size_t position);
	//const short* readData(size_t count);
	const short operator[](size_t pos);
};

