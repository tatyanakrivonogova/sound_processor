#pragma once
#include <vector>
#include <string>
#include "Converter.h"
#include "Stream.h"

class muteConverter : public Converter
{
private:
	FILE* fin = nullptr;
	FILE* fout = nullptr;
public:
	muteConverter() = default;
	~muteConverter() = default;

	Stream convert(std::vector<std::string>&, std::vector<double>&, std::shared_ptr<std::string>) override;
	void whatAreYouDoing(FILE*) override;
	void setArgs(std::vector<std::string>&, std::vector<double>&, std::string&, double&, double&);
	void checkArgs(Stream&, size_t&, size_t&, size_t&, double&, double&);
	void prepareStreams(std::shared_ptr<std::string>, std::string&, Stream&, Stream&);
};

