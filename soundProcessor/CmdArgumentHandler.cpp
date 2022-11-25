#include "CmdArgumentHandler.h"

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

bool CmdArgumentHandler::checkFileFormat(std::string& str, std::string expectedFormat) {
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

int CmdArgumentHandler::getCmdArgs(int argc, char** argv, std::string& configFile, std::string& outputFile, std::vector<std::string>& inputFiles, bool& help) {
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