#include "readBuffer.h"
#include "Buffer.h"
#include <iostream>

readBuffer::readBuffer(size_t BUFF_SIZE, FILE* file, size_t position) : Buffer(BUFF_SIZE, file, position) {
	fullness = fread(buff, sizeof(short), BUFF_SIZE, file);
}

const short readBuffer::operator[](size_t pos) {
	//if (pos < 0) {
	//	throw std::invalid_argument("Wrong index");
	//}
	if (pos >= current_position and pos < (current_position + fullness)) {
		return buff[pos-current_position];
	}
	else {
		fseek(file, begin_position + 2*pos, SEEK_SET);
		fullness = fread(buff, sizeof(short), BUFF_SIZE, file);
		current_position = pos;//!!!!!!!!!!!!!!!!!!
		/*if (pos >= current_position and pos < (current_position + fullness)) {
			return buff[pos - current_position];
		}*/
		return buff[pos - current_position];
		//else {
		//	std::cout << "Not finded position!" << std::endl;
		//	return -1;
		//}
	}
}