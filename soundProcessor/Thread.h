#pragma once
#include "WAVheader.h"
#include <memory>
#include <vector>
#include <string>

class Thread
{
public:
	Thread(WAVheader header, unsigned int data = 0, std::shared_ptr<std::string> source = nullptr) : header(header), data(data) {}
	Thread(std::shared_ptr<std::string> source = nullptr) : source(source) {}
	~Thread() {}

	Thread(const Thread&);
	Thread(Thread&&) noexcept;

	Thread& operator=(const Thread&);
	Thread& operator=(Thread&&) noexcept;

	WAVheader& getHeader();
	unsigned int getData();
	std::shared_ptr<std::string> getSource();
	
	void setHeader(WAVheader& newHeader);
	void setData(unsigned int newData);
	void setSource(std::shared_ptr<std::string> newSource);

private:
	WAVheader header;
	unsigned int data = 0;
	std::shared_ptr<std::string> source;
};