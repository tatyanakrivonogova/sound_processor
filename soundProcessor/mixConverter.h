#pragma once
#include <vector>
#include <cstdio>
#include "Converter.h"
#include "Stream.h"

class mixConverter : public Converter
{
private:

public:
	mixConverter() = default;
	~mixConverter() = default;

	Stream convert(std::vector<std::string>, std::vector<double>, std::shared_ptr<std::string>) override;
	void whatAreYouDoing(FILE*) override;
};

