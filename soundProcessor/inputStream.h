#pragma once
#include "Stream.h"
#include <fstream>
#include <string>

class inputStream {
private:
	std::string& inputFile;
	Stream& stream;

public:
	inputStream(std::string& inputFile, Stream& stream) : inputFile(inputFile), stream(stream) {}
	int input();
};