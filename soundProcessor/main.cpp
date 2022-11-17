#include <iostream>
#include <fstream>
#include <string>
#include "dsound.h"
#include <iosfwd>
#include "WAVheader.h"
#include "Thread.h"
#include "inputThread.h"
#include "outputThread.h"
#include "muteConverter.h"

int main() {

	std::string inputFile = "Ring08.wav";
	Thread thread;
	inputThread inputThread1(inputFile, thread);
	inputThread1.input();

	std::string outputFile = "new_Ring08.wav";
	

	muteConverter mute(thread, 1,  4);

	thread = mute.convert();
	outputThread outputThread1(outputFile, thread);
	outputThread1.output();

	return 0;
}