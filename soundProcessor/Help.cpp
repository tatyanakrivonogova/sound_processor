#include "Help.h"
#include "Factory.h"
#include "Converter.h"

extern AbstractFactory<Converter, std::string> ConverterFactory;

void Help::getHelp() {
	FILE* file;
	fopen_s(&file, "help.txt", "w");

	std::map <std::string, Converter* (*)()> registered = ConverterFactory.getMap();

	for (auto it = registered.begin(); it != registered.end(); ++it) {
		std::shared_ptr<Converter> currentTypeConverter = std::shared_ptr<Converter>((it->second)());
		currentTypeConverter->whatAreYouDoing(file);
	}
}