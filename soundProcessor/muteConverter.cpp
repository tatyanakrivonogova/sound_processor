#include "Factory.h"
#include "muteConverter.h"
#include "Converter.h"
#include "Factory.h"
#include "Thread.h"
#include "readBuffer.h"
#include "writeBuffer.h"
#include "outputHeader.h"
#include "inputThread.h"
#include <fstream>
#include <iostream>
#include <string>
#define BUFF_SIZE 1000

extern AbstractFactory<Converter, std::string> ConverterFactory;

namespace {
	Converter* createMuteConverter(std::vector<std::string> threads, std::vector<unsigned int> parameters) {
		return new muteConverter(threads, parameters);
	}
	
	const bool registered = ConverterFactory.Register("mute", createMuteConverter);
}

muteConverter::muteConverter(std::vector<std::string> threadFiles, std::vector<unsigned int> parameters) {
	threadFile = threadFiles[0];
	if (parameters.size() == 2) {
		time_begin = parameters[0];
		duration = parameters[1]; 
	}
	else if (parameters.size() == 1) {
		//throw std::runtime_error("Expected mixing start time");
		time_begin = 0;
		duration = parameters[0];
	}
	else if (parameters.size() == 0) {
		throw std::runtime_error("Expected muting duration");
	}
	else {
		throw std::invalid_argument("Extra arguments for muting");
	}
}

Thread muteConverter::convert() {

	Thread thread(std::make_shared<std::string>(threadFile));
	inputThread inputThread1(threadFile, thread);
	inputThread1.input();

	FILE* fin;
	fopen_s(&fin, (*thread.getFile()).c_str(), "rb");


	readBuffer readBuff(BUFF_SIZE, fin, thread.getData());


	Thread newThread(thread);
	std::string newFile = "muted_" + (*thread.getFile());
	newThread.setFile(std::make_shared<std::string>(newFile));

	FILE* fout;
	fopen_s(&fout, (*newThread.getFile()).c_str(), "wb");


	outputHeader outputHeader(fout, newThread.getHeader());
	outputHeader.output();


	writeBuffer writeBuf(BUFF_SIZE, fout, newThread.getData());
	
	size_t data_size = (thread.getHeader().get_chunk_size() - thread.getData()) / 2;
	size_t begin = time_begin * 44100;
	if (begin > data_size) {
		throw std::invalid_argument("Unavailable argument of begin_time");
	}

	size_t end = (time_begin + duration) * 44100;
	if (end > data_size) {
		throw std::invalid_argument("Unavailable argument of duration");
	}


	//before begin
	for (size_t i = 0; i < begin; ++i) {
		writeBuf >> readBuff[i];
	}

	//changing
	for (size_t i = begin; i < end; ++i) {
		writeBuf >> 0;
	}

	//after end
	for (size_t i = end; i < data_size; ++i) {
		writeBuf >> readBuff[i];
	}

	fclose(fin);
	fclose(fout);
	return newThread;
}