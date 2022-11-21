#include "readBuffer.h"
#include "Buffer.h"
#include <iostream>

const short readBuffer::operator[](size_t pos) {
	//pos += begin_position;
	if (pos < 0) {
		std::cout << "Wrong argument of position!" << std::endl; // make exception
		return -1;
	}
	if (pos >= current_position and pos < (current_position + BUFF_SIZE)) {
		return buff[pos-current_position];
	}
	else {
		fseek(file, begin_position + 2*pos, SEEK_SET);
		//std::cout << ftell(file) << std::endl;
		if (fread(buff, sizeof(short), BUFF_SIZE, file) != BUFF_SIZE) {
			std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
		}
		current_position = pos;
		if (pos >= current_position and pos < (current_position + BUFF_SIZE)) {
			return buff[pos - current_position];
		}
		else {
			std::cout << "Not finded position!" << std::endl;
			return -1;
		}
	}
}