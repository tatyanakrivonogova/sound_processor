#include "writeBuffer.h"
#include "Buffer.h"
#include <iostream>

void writeBuffer::operator>>(const short value) {
	buff[current_position] = value;
	++current_position;
	if (current_position == BUFF_SIZE) {
		fwrite(buff, sizeof(short), BUFF_SIZE, file);
		current_position = 0;
	}
}