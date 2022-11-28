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
#include "Help.h"

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

	CmdArgumentHandler CmdArgumentHandler;

	try {
		CmdArgumentHandler.getCmdArgs(argc, argv, configFile, outputFile, inputFiles, help);
	}
	catch (std::invalid_argument const& ex) {
		std::cerr << ex.what() << '\n';
		return -1;
	}

	if (help == true) {
		Help help;
		help.getHelp();
	}

	if (outputFile.empty() and configFile.empty() and inputFiles.empty()) {
		return 0;
	}
	std::vector<std::vector<std::string> > config;
	readConfig readConfig(configFile, inputFiles);
	try {
		readConfig.read(config);
	}
	catch (std::runtime_error const& ex) {
		std::cerr << ex.what() << '\n';
		return -1;
	}

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

		std::shared_ptr<Converter> current_converter = std::shared_ptr<Converter>(ConverterFactory.CreateObject(config[i][0]));
		
		try {
			
			if (i == config.size() - 1) {
				thread = current_converter->convert(threads, parameters, std::make_shared<std::string>(outputFile));
			}
			else {
				thread = current_converter->convert(threads, parameters);
			}
		}
		catch (std::runtime_error const& ex) {
			std::cerr << ex.what() << '\n';
			return -1;
		}
		catch (std::invalid_argument const& ex) {
			std::cerr << ex.what() << '\n';
		}
	}

	return 0;
}