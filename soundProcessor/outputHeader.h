#pragma once
#include <string>
#include "WAVheader.h"

class outputHeader
{
private:
	//std::string& outputFile;
	FILE* fout;
	WAVheader& header;

public:
	outputHeader(FILE* fout, WAVheader& header) : fout(fout), header(header) {}
	void output();
};

