#pragma once
#include <string>
#include <vector>

class readConfig
{
private:
	std::string& configFile;
public:
	readConfig(std::string& configFile) : configFile(configFile) {}
	~readConfig() {}
	void read(std::vector<std::vector<std::string> >&);
};

