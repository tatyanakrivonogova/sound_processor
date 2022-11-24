#include <iostream>
#include <fstream>
#include <string>
#include <iosfwd>
#include <memory>
#include <vector>

#include "dsound.h"
#include "Factory.h"
#include "WAVheader.h"
#include "Thread.h"
#include "inputThread.h"
#include "outputThread.h"
#include "muteConverter.h"
#include "readConfig.h"

AbstractFactory<Converter, std::string> ConverterFactory;

int main() {

	std::string inputFile = "Ring08.wav";
	////std::string inputFile = "funkorama.wav";


	std::string configFile = "config.txt";
	std::vector<std::vector<std::string> > config;
	readConfig readConfig(configFile);
	readConfig.read(config);

	Thread thread;

	for (size_t i = 0; i < config.size(); ++i) {
		if (config[i][0] == "mute") {
			std::vector<std::string> threads;
			threads.push_back(inputFile);
			std::shared_ptr<Converter> mute = std::shared_ptr<Converter>(ConverterFactory.CreateObject("mute", threads, std::vector<unsigned int>({ 1, 4 })));
			thread = mute->convert();
		}
		else if (config[i][0] == "mix") {
			std::vector<std::string> threads;
			threads.push_back(inputFile);
			threads.push_back(config[i][1]);
			muteConverter mute(threads, std::vector<unsigned int>({ 1, 4 }));
			thread = mute.convert();
		}
		
	}

	return 0;
}