#pragma once
#include "Buffer.h"

class writeBuffer : public Buffer
{
public:
	writeBuffer(size_t BUFF_SIZE, FILE* file, size_t begin_position) : Buffer(BUFF_SIZE, file, begin_position) {}
	void operator>>(const short value);
};