#pragma once
#include <vector>
#include <string>
#include "Converter.h"
#include "Stream.h"

class muteConverter : public Converter
{
private:

public:
	muteConverter() = default;
	~muteConverter() = default;

	Stream convert(std::vector<std::string>, std::vector<double>, std::shared_ptr<std::string>) override;
	void whatAreYouDoing(FILE*) override;
};

