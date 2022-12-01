#pragma once
#include "Stream.h"
#include <fstream>
#include <string>

class outputStream {
private:
	std::string& outputFile;
	Stream& stream;

public:
	outputStream(std::string& outputFile, Stream& stream) : outputFile(outputFile), stream(stream) {}
	int output();
};