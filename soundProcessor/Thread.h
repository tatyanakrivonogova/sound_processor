#pragma once
#include "WAVheader.h"
#include <memory>
#include <vector>

class Thread
{
public:
	Thread() {}
	Thread(WAVheader header, std::vector<char> data) : header(header), data(data) {}
	~Thread() {}

	Thread(const Thread&);
	Thread(Thread&&) noexcept;

	Thread& operator=(const Thread&);
	Thread& operator=(Thread&&) noexcept;

	WAVheader& getHeader();
	std::vector<char>& getData();
	void setData(std::vector<char>& newData);

private:
	WAVheader header;
	//std::shared_ptr <char> data = nullptr;
	std::vector<char> data;
};