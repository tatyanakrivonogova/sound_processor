#pragma once
#include "WAVheader.h"
#include <memory>
#include <vector>
#include <string>

class Thread
{
public:
	Thread(WAVheader header, unsigned int data = 0, unsigned int number_of_samples = 0, std::shared_ptr<std::string> file = nullptr) : header(header), data(data) {}
	Thread(std::shared_ptr<std::string> file = nullptr) : file(file) {}
	~Thread() {}

	Thread(const Thread&);
	Thread(Thread&&) noexcept;

	Thread& operator=(const Thread&);
	Thread& operator=(Thread&&) noexcept;

	WAVheader& getHeader();
	const unsigned int getData();
	const unsigned int getNumberOfSamples();
	std::shared_ptr<std::string> getFile();
	
	void setHeader(const WAVheader& newHeader);
	void setData(unsigned int newData);
	void setNumberOfSamples(unsigned int number_of_samples);
	void setFile(std::shared_ptr<std::string> newFile);

private:
	WAVheader header;
	unsigned int data = 0;
	unsigned int number_of_samples = 0;
	std::shared_ptr<std::string> file;
};