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

	//std::string inputFile = "Ring08.wav";
	std::string inputFile = "funkorama.wav";


	std::string configFile = "config.txt";
	std::vector<std::vector<std::string> > config;
	readConfig readConfig(configFile);
	readConfig.read(config);

	Thread thread;

	for (size_t i = 0; i < config.size(); ++i) {
		if (config[i][0] == "mute") {

			std::vector<std::string> threads;
			threads.push_back(inputFile);

			std::vector<unsigned int> time_args;
			for (int j = 1; j < config[i].size(); ++j) {
				time_args.push_back(stoi(config[i][j]));
			}

			std::shared_ptr<Converter> mute = std::shared_ptr<Converter>(ConverterFactory.CreateObject("mute", threads, time_args));
			thread = mute->convert();
		}
		else if (config[i][0] == "mix") {

			std::vector<std::string> threads;
			//threads.push_back(inputFile);
			threads.push_back(*thread.getFile());
			threads.push_back(config[i][1]);

			std::vector<unsigned int> time_args;
			for (int j = 2; j < config[i].size(); ++j) {
				time_args.push_back(stoi(config[i][j]));
			}
			
			std::shared_ptr<Converter> mix = std::shared_ptr<Converter>(ConverterFactory.CreateObject("mix", threads, time_args));
			thread = mix->convert();
		}
		
	}

	return 0;
}