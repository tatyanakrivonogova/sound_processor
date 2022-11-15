#pragma once
#include "WAVheader.h"

class Thread
{
public:
	Thread() {}
	Thread(WAVheader header, char* data) : header(header), data(data) {}
	~Thread() {}

	WAVheader& getHeader();
	char* getData();
	void setData(char* newData);

private:
	WAVheader header;
	char* data = nullptr;
};