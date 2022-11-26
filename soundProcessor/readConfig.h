#pragma once
#include <string>
#include <vector>

class readConfig
{
private:
	std::string& configFile;
	std::vector<std::string>& inputFiles;
public:
	readConfig(std::string& configFile, std::vector<std::string>& inputFiles) : configFile(configFile), inputFiles(inputFiles){}
	~readConfig() {}
	void read(std::vector<std::vector<std::string> >&);
};

