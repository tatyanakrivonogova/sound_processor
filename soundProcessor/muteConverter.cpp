#include "muteConverter.h"
#include "Converter.h"
#include "Thread.h"
#include <fstream>

Thread muteConverter::convert() {
	//std::fstream source;
	FILE* fin;
	fopen_s(&fin, (*thread.getFile()).c_str(), "rb");
	Thread newThread;

	///*for (unsigned int i = time_begin*44100; i < (time_begin + duration)*44100; ++i) {
	//	(thread.getData())[i] = '\0';
	//}*/

	//for (unsigned int i = 0; i < thread.getData().size(); ++i) {
	//	(newThread.getData())[i] = '\0';
	//}

	return newThread;
}