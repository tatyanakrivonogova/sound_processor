#pragma once
#include "Converter.h"
#include "Thread.h"

class slowConverter : public Converter
{
private:

public:
	slowConverter() = default;
	~slowConverter() = default;

	Thread convert(std::vector<std::string>, std::vector<unsigned int>, std::shared_ptr<std::string>) override;
	void whatAreYouDoing(FILE*) override;
};

