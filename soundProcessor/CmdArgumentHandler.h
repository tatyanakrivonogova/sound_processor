#pragma once
#include <string>
#include <vector>
class CmdArgumentHandler
{
private:
	std::string& outputFile;
	std::vector<std::string>& inputFiles;
	std::string& configFile;
	bool& help;

	void checkFileFormat(std::string&, std::string);
	std::string getCmdArgument(char**, size_t);

public:
	CmdArgumentHandler(std::string& outputFile, std::vector<std::string>& inputFiles, std::string& configFile, bool& help) :
		outputFile(outputFile), inputFiles(inputFiles), configFile(configFile), help(help) {}
	void getCmdArgs(int, char**, std::string&, std::string&, std::vector<std::string>&, bool&);
};

