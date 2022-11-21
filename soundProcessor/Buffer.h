#pragma once
#include <cstdio>

class Buffer
{
protected:
	FILE* file;
	size_t begin_position;
	short* buff = nullptr;
	size_t BUFF_SIZE;
	size_t current_position = 0;

public:
	Buffer(size_t BUFF_SIZE, FILE* file = nullptr, size_t begin_position = 0);
	~Buffer();

	FILE* getFile();
	size_t getBeginPosition();
	size_t getBUFF_SIZE();

	void setFile(FILE* file);
	void setBeginPosition(size_t begin_position);
};

