#pragma once
#include <vector>
#include <string>
#include "Converter.h"
#include "Stream.h"

class reverbConverter : public Converter
{
private:

public:
	reverbConverter() = default;
	~reverbConverter() = default;

	Stream convert(std::vector<std::string>, std::vector<double>, std::shared_ptr<std::string>) override;
	void whatAreYouDoing(FILE*) override;
};

