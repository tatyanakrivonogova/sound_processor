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

size_t getNumberOfThreads(std::vector<std::string> command) {
	size_t current_arg = 1; // miss name of converter
	size_t count = 0;
	while (command[current_arg].find(".wav") != std::string::npos) {
		++current_arg;
		++count;
	}
	return count;
}

std::string getCmdArgument( char** argv, size_t indexArgument) {
	std::string arg = argv[indexArgument];
	while (isspace(arg.front())) {
		arg.erase(0, 1);
	}
	while (isspace(arg.back())) {
		arg.erase(arg.length() - 1, 1);
	}
	return arg;
}

bool checkFileFormat(std::string& str, std::string expectedFormat) {
	std::string format;

	std::string::iterator pos = find(str.begin(), str.end(), '.');
	if (pos == str.end()) {
		return 1;
	}
	++pos;
	while (pos != str.end()) {
		format += (*pos);
		++pos;
	}
	if (format == expectedFormat) {
		return 0;
	}
	else {
		return 1;
	}
}

int getCmdArgs(int argc, char** argv, std::string& configFile, std::string& outputFile, std::vector<std::string>& inputFiles, bool& help) {
	int indexArgument = 1;
	bool output = false;

	while (indexArgument < argc) {
		//std::string argument = argv[currentArgument];
		std::string currentArgument = getCmdArgument(argv, indexArgument);
		++indexArgument;

		if (currentArgument == "-h") {
			help = true;
		}
		else if (currentArgument == "-c") {
			if (indexArgument == argc) {
				return 1; //throw exception
			}
			configFile = getCmdArgument(argv, indexArgument);
			++indexArgument;
			if (checkFileFormat(configFile, "txt") == 1) {
				return 1;
			}
		}
		else {
			//std::string inputFile = getCmdArgument(argv, currentArgument);
			if (checkFileFormat(currentArgument, "wav") == 1) {
				return 1; //throw exception
			}
			if (output == false) {
				outputFile = std::move(currentArgument);
				output = true;
			}
			else {
				inputFiles.push_back(std::move(currentArgument));
			}
		}
	}
	return 0;
}

int main(int argc, char** argv) {

	std::string outputFile;
	std::vector<std::string> inputFiles;
	std::string configFile;
	bool help = false;

	if (getCmdArgs(argc, argv, configFile, outputFile, inputFiles, help) == 1) {
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