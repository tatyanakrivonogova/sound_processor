#pragma once
#include <vector>
#include <string>
#include "Converter.h"
#include "Thread.h"

class reverbConverter : public Converter
{
private:

public:
	reverbConverter() = default;
	~reverbConverter() = default;

	Thread convert(std::vector<std::string>, std::vector<unsigned int>, std::shared_ptr<std::string>) override;
	void whatAreYouDoing(FILE*) override;
};

