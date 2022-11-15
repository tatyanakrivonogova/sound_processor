#pragma once
#include "Thread.h"
#include <fstream>
#include <string>

class inputThread {
private:
	std::string& inputFile;
	Thread& thread;

public:
	inputThread(std::string& inputFile, Thread& thread) : inputFile(inputFile), thread(thread) {}
	int input();
};