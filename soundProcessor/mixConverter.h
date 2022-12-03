#pragma once
#include <vector>
#include <cstdio>
#include "Converter.h"
#include "Stream.h"

class mixConverter : public Converter
{
private:
	FILE* fin1 = nullptr;
	FILE* fin2 = nullptr;
	FILE* fout = nullptr;
public:
	mixConverter() = default;
	~mixConverter() = default;

	Stream convert(std::vector<std::string>&, std::vector<double>&, std::shared_ptr<std::string>) override;
	void whatAreYouDoing(FILE*) override;
	void setArgs(std::vector<std::string>&, std::vector<double>&, std::string&, std::string&, double&);
	void checkArgs(Stream&, Stream&, size_t&, size_t&, size_t&, double&);
	void prepareStreams(std::shared_ptr<std::string>, std::string&, std::string&, Stream&, Stream&, Stream&);
};

