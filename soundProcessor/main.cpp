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
#include "CmdArgumentHandler.h"

AbstractFactory<Converter, std::string> ConverterFactory;

size_t getNumberOfThreads(std::vector<std::string> command) {
	size_t current_arg = 1; // miss name of converter
	size_t count = 0;
	while (command[current_arg].find(".wav") != std::string::npos) {
		++current_arg;
		++count;
	}
	return count;
}



int main(int argc, char** argv) {

	std::string outputFile;
	std::vector<std::string> inputFiles;
	std::string configFile;
	bool help = false;

	CmdArgumentHandler CmdArgumentHandler(outputFile, inputFiles, configFile, help);

	if (CmdArgumentHandler.getCmdArgs(argc, argv, configFile, outputFile, inputFiles, help) == 1) {
		std::cout << "BAD" << std::endl;
		return -1;
	}

	//std::string inputFile = "Ring08.wav";
	//std::string inputFile = "funkorama.wav";


	//std::string configFile = "config.txt";
	std::vector<std::vector<std::string> > config;
	readConfig readConfig(configFile);
	readConfig.read(config);

	Thread thread(std::make_shared<std::string>(inputFiles[0]));

	for (size_t i = 0; i < config.size(); ++i) {

		size_t threads_number = getNumberOfThreads(config[i]);
		std::vector<std::string> threads;
		threads.push_back(*thread.getFile());
		for (size_t j = 1; j < threads_number+1; ++j) { //miss name of converter
			threads.push_back(config[i][j]);
		}

		std::vector<unsigned int> time_args;
		for (size_t j = threads_number+1; j < config[i].size(); ++j) { // miss name of converter and input threads
			time_args.push_back(stoi(config[i][j]));
		}

		std::shared_ptr<Converter> current_converter = std::shared_ptr<Converter>(ConverterFactory.CreateObject(config[i][0], threads, time_args));
		thread = current_converter->convert();
	}

	return 0;
}