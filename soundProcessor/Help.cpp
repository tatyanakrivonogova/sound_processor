#include "Help.h"
#include "Factory.h"
#include "Converter.h"

extern AbstractFactory<Converter, std::string> ConverterFactory;

void Help::getHelp() {
	//std::map <std::string, Converter* (*)(std::vector<std::string>, std::vector<unsigned int>)> registered = ConverterFactory.getMap();
	//for (auto typeConverter : registered) {

	//}

}