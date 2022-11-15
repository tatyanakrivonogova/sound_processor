#pragma once
#include "Thread.h"
#include <fstream>
#include <string>

class outputThread {
private:
	std::string& outputFile;
	Thread& thread;

public:
	outputThread(std::string& outputFile, Thread& thread) : outputFile(outputFile), thread(thread) {}
	int output();
};