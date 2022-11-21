#include "writeBuffer.h"
#include "Buffer.h"
#include <iostream>

//void writeBuffer::writeData(const short* data, size_t count) {
//	if (used_data == BUFF_SIZE) {
//		fwrite(buff, sizeof(short), BUFF_SIZE, file);
//		used_data = 0;
//	}
//	for (size_t i = used_data; i < used_data + count; ++i) {
//		buff[i] = data[i];
//	}
//}

void writeBuffer::operator>>(const short value) {
	buff[current_position] = value;
	++current_position;
	if (current_position == BUFF_SIZE) {
		fwrite(buff, sizeof(short), BUFF_SIZE, file);
		/*std::cout << " read " << std::endl;
		for (int i = 0; i < 10; ++i) {
			std::cout << buff[i] << " ";
		}
		std::cout << std::endl;*/
		current_position = 0;
	}
}