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
#include "readConfig.h"

int main() {

	std::string inputFile = "Ring08.wav";
	////std::string inputFile = "funkorama.wav";
	Thread thread(std::make_shared<std::string>(inputFile));
	inputThread inputThread1(inputFile, thread);
	inputThread1.input();

	

	//muteConverter mute(thread, 1,  4);
	//thread = mute.convert();

	std::string configFile = "config.txt";
	std::vector<std::vector<std::string> > config;
	readConfig readConfig(configFile);
	readConfig.read(config);

	for (size_t i = 0; i < config.size(); ++i) {
		if (config[i][0] == "mute") {
			muteConverter mute(thread, 1, 4);
			thread = mute.convert();
		}
	}

	return 0;
}