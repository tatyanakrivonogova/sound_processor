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
	while (current_arg < command.size() and command[current_arg].find(".wav") != std::string::npos) {
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

	try {
		CmdArgumentHandler.getCmdArgs(argc, argv, configFile, outputFile, inputFiles, help);
	}
	catch (std::invalid_argument const& ex) {
		std::cerr << ex.what() << '\n';
		return -1;
	}

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

		std::vector<unsigned int> parameters;
		for (size_t j = threads_number+1; j < config[i].size(); ++j) { // miss name of converter and input threads
			try {
				parameters.push_back(stoi(config[i][j]));
			}
			catch (std::invalid_argument const& ex) {
				std::cerr << ex.what() << '\n';
				return -1;
			}
		}

		std::shared_ptr<Converter> current_converter;
		try {
			current_converter = std::shared_ptr<Converter>(ConverterFactory.CreateObject(config[i][0], threads, parameters));
		}
		catch (std::runtime_error const& ex) {
			std::cerr << ex.what() << '\n';
			return -1;
		}
		catch (std::invalid_argument const& ex) {
			std::cerr << ex.what() << '\n';
		}
		
		try {
			thread = current_converter->convert();
		}
		catch (std::runtime_error const& ex) {
			std::cerr << ex.what() << '\n';
			return -1;
		}
	}

	return 0;
}