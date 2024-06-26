#include "readConfig.h"
#include "Factory.h"
#include <fstream>
#include <string>
#include <vector>

bool isComment(std::string str) {
	size_t pos = 0;
	while (pos < str.length() and isspace(str[pos])) {
		++pos;
	}
	return (str[pos] == '#') ? true : false;
}

bool isDigit(char c) {
	return c >= '0' and c <= '9';
}

size_t getStreamNumber(std::string arg) {
	std::string number;
	size_t pos = 0;
	while (pos < arg.length() and !isDigit(arg[pos])) {
		++pos;
	}
	while (pos < arg.length() and isDigit(arg[pos])) {
		number += arg[pos];
		++pos;
	}
	return stoi(number);
}

void getCommand(std::vector<std::string>& command, std::string str) {

	std::string arg;
	size_t pos = 0;
	while (true) {
		while (pos < str.length() and isspace(str[pos])) {
			++pos;
		}

		while (pos < str.length() and !isspace(str[pos])) {
			arg += str[pos];
			++pos;
		}

		if (!arg.empty()) {
			command.push_back(arg);
			arg.clear();
		}
		if (pos == str.length()) {
			break;
		}
	}
}

void readConfig::read(std::vector<std::vector<std::string> >& config) {
	std::ifstream file;
	file.open(configFile);
	setlocale(LC_ALL, "Russian");
	std::string str;
	
	std::vector<std::string> command;

	while (true) {
		std::getline(file, str);
		if (str.empty()) {
			break;
		}
		if (!isComment(str)) {
			getCommand(command, str);

			for (std::vector<std::string>::iterator it = command.begin(); it < command.end(); ++it) {
				if ((*it).front() == '$') {
					size_t stream_number = getStreamNumber(*it);
					if (stream_number > inputFiles.size()) {
						throw std::runtime_error("Wrong reference to stream");
					}
					*it = inputFiles[stream_number - 1];
				}
			}
			config.push_back(command);
			command.clear();
		}
	}
}
