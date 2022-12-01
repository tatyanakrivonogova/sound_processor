#pragma once
#include "WAVheader.h"
#include <memory>
#include <vector>
#include <string>

class Stream
{
public:
	Stream(WAVheader header, unsigned int data = 0, unsigned int number_of_samples = 0, std::shared_ptr<std::string> file = nullptr) : header(header), data(data) {}
	Stream(std::shared_ptr<std::string> file = nullptr) : file(file) {}
	~Stream() {}

	Stream(const Stream&);
	Stream(Stream&&) noexcept;

	Stream& operator=(const Stream&);
	Stream& operator=(Stream&&) noexcept;

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