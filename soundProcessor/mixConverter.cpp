#include "Factory.h"
#include "mixConverter.h"
#include "Converter.h"
#include "Factory.h"
#include "Thread.h"
#include "readBuffer.h"
#include "writeBuffer.h"
#include "outputHeader.h"
#include "inputThread.h"
#include <fstream>
#include <iostream>
#define BUFF_SIZE 1000

extern AbstractFactory<Converter, std::string> ConverterFactory;

namespace {
	Converter* createMixConverter(std::vector<std::string> threadFiles, std::vector<unsigned int> parameters) {
		return new mixConverter(threadFiles, parameters);
	}

	const bool registered = ConverterFactory.Register("mix", createMixConverter);
}

mixConverter::mixConverter(std::vector<std::string> threadFiles, std::vector<unsigned int> parameters) {
	threadFile1 = threadFiles[0];

	if (threadFiles.size() == 2) {
		threadFile2 = threadFiles[1];
	}
	else if (threadFiles.size() < 2) {
		throw std::runtime_error("Expected second input file");
	}
	else {
		throw std::invalid_argument("Extra arguments for mixing");
	}

	if (!parameters.empty()) {
		time_begin = parameters[0];
	}
	else {
		//throw std::invalid_argument("Expected mixing start time");
		time_begin = 0;
	}
}

Thread mixConverter::convert() {


	Thread thread1(std::make_shared<std::string>(threadFile1));
	inputThread inputThread1(threadFile1, thread1);
	inputThread1.input();

	Thread thread2(std::make_shared<std::string>(threadFile2));
	inputThread inputThread2(threadFile2, thread2);
	inputThread2.input();


	size_t data_size = thread1.getNumberOfSamples();
	size_t begin = time_begin * 44100;
	if (begin > data_size) {
		throw std::invalid_argument("Unavailable argument of begin_time");
	}
	size_t end = (thread1.getNumberOfSamples() - begin <= thread2.getNumberOfSamples())
		? thread1.getNumberOfSamples() : begin + thread2.getNumberOfSamples();
	if (end > data_size) {
		throw std::invalid_argument("Unavailable argument of duration");
	}


	FILE* fin1;
	fopen_s(&fin1, (*thread1.getFile()).c_str(), "rb");

	FILE* fin2;
	fopen_s(&fin2, (*thread2.getFile()).c_str(), "rb");


	readBuffer readBuff1(BUFF_SIZE, fin1, thread1.getData());
	readBuffer readBuff2(BUFF_SIZE, fin2, thread2.getData());


	Thread newThread;
	std::string newFile = "mixed_" + (*thread1.getFile()) + '_' + (*thread2.getFile());
	newThread.setFile(std::make_shared<std::string>(newFile));
	newThread.setHeader(thread1.getHeader());
	newThread.setData(thread1.getData());


	FILE* fout;
	fopen_s(&fout, (*newThread.getFile()).c_str(), "wb");

	outputHeader outputHeader(fout, newThread.getHeader());
	outputHeader.output();

	writeBuffer writeBuf(BUFF_SIZE, fout, newThread.getData());


	//before begin
	for (size_t i = 0; i < begin; ++i) {
		writeBuf >> readBuff1[i];
	}

	//changing
	for (size_t i1 = begin, i2 = 0; i1 < (end-begin)/2; ++i1, ++i2) {
		writeBuf >> readBuff1[i1] + readBuff2[i2];
	}

	//after end
	for (size_t i = end; i < data_size; ++i) {
		writeBuf >> readBuff1[i];
	}

	fclose(fin1);
	fclose(fin2);
	fclose(fout);
	return newThread;
}