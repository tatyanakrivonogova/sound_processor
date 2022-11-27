#pragma once
#include <vector>
#include <string>
#include "Converter.h"
#include "Thread.h"

class muteConverter : public Converter
{
private:

public:
	muteConverter() = default;
	~muteConverter() = default;

	Thread convert(std::vector<std::string>, std::vector<unsigned int>, std::shared_ptr<std::string>) override;
	void whatAreYouDoing(FILE*) override;
};

