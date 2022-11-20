#include <iostream>
#include <fstream>
#include <string>
#include "dsound.h"
#include <iosfwd>
#include <memory>
#include "WAVheader.h"
#include "Thread.h"
#include "inputThread.h"
#include "outputThread.h"
#include "muteConverter.h"

int main() {

	std::string inputFile = "Ring08.wav";
	Thread thread(std::make_shared<std::string>(inputFile));
	inputThread inputThread1(inputFile, thread);
	inputThread1.input();

	std::string outputFile = "new_Ring08.wav";
	

	//muteConverter mute(thread, 1,  4);

	//thread = mute.convert();
	outputThread outputThread1(outputFile, thread);
	outputThread1.output();

	Thread thread2(std::make_shared<std::string>(outputFile));
	inputThread inputThread2(inputFile, thread2);
	inputThread2.input();
	return 0;
}