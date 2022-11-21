#include "Buffer.h"
#include <cstdio>
#include <iostream>

Buffer::Buffer(size_t BUFF_SIZE, FILE* file, size_t begin_position) : BUFF_SIZE(BUFF_SIZE), file(file), begin_position(begin_position) {
	buff = new short[BUFF_SIZE];
	
	fseek(file, begin_position, SEEK_SET);
}

Buffer::~Buffer() {
	delete[] buff;
}

FILE* Buffer::getFile() {
	return file;
}

size_t Buffer::getBeginPosition() {
	return begin_position;
}

size_t Buffer::getBUFF_SIZE() {
	return BUFF_SIZE;
}

void Buffer::setFile(FILE* file) {
	file = file;
}

void Buffer::setBeginPosition(size_t begin_position) {
	begin_position = begin_position;
}

