#pragma once
#include <string>
#include <vector>
class CmdArgumentHandler
{
private:

	void checkFileFormat(std::string&, std::string);
	std::string getCmdArgument(char**, size_t);

public:
	void getCmdArgs(int, char**, std::string&, std::string&, std::vector<std::string>&, bool&);
};

