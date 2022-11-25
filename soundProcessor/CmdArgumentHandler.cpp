#include "CmdArgumentHandler.h"
#include <stdexcept>

std::string CmdArgumentHandler::getCmdArgument(char** argv, size_t indexArgument) {
	std::string arg = argv[indexArgument];
	while (isspace(arg.front())) {
		arg.erase(0, 1);
	}
	while (isspace(arg.back())) {
		arg.erase(arg.length() - 1, 1);
	}
	return arg;
}

void CmdArgumentHandler::checkFileFormat(std::string& str, std::string expectedFormat) {
	std::string format;

	std::string::iterator pos = find(str.begin(), str.end(), '.');
	if (pos == str.end()) {
		throw std::invalid_argument("Wrong cmd argument");
	}
	++pos;
	while (pos != str.end()) {
		format += (*pos);
		++pos;
	}
	if (format != expectedFormat) {
		throw std::invalid_argument("Wrong file format");
	}
}

void CmdArgumentHandler::getCmdArgs(int argc, char** argv, std::string& configFile, std::string& outputFile, std::vector<std::string>& inputFiles, bool& help) {
	int indexArgument = 1;
	bool output = false;

	while (indexArgument < argc) {
		std::string currentArgument = getCmdArgument(argv, indexArgument);
		++indexArgument;

		if (currentArgument == "-h") {
			help = true;
		}
		else if (currentArgument == "-c") {
			if (indexArgument == argc) {
				throw std::invalid_argument("More arguments were expected");
			}
			configFile = getCmdArgument(argv, indexArgument);
			++indexArgument;
			try {
				checkFileFormat(configFile, "txt");
			}
			catch (std::invalid_argument const& ex) {
				throw ex;
			}
		}
		else {
			try {
				checkFileFormat(currentArgument, "wav");
			}
			catch (std::invalid_argument const& ex) {
				throw ex;
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

	if (outputFile.empty()) {
		throw std::invalid_argument("No output file");
	}
	if (configFile.empty()) {
		throw std::invalid_argument("No config file");
	}
	if (inputFiles.empty()) {
		throw std::invalid_argument("No input file");
	}

}