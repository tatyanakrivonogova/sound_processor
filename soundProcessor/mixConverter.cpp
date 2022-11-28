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
	Converter* createMixConverter() {
		return new mixConverter();
	}

	const bool registered = ConverterFactory.Register("mix", createMixConverter);
}

void mixConverter::whatAreYouDoing(FILE* fout) {
	std::string info;
	info += "mixConverter\n";
	info += "\tcommand: mix second thread [start]\n";
	info += "\tmixing of the main and second streams starting from 'start' second (default [start = 0])\n";
	fseek(fout, 0, SEEK_END);
	fwrite(info.data(), sizeof(char), info.size(), fout);
}

Thread mixConverter::convert(std::vector<std::string> threadFiles, std::vector<unsigned int> parameters, std::shared_ptr<std::string> outputFile = nullptr) {
	std::string threadFile1;
	std::string threadFile2;
	unsigned int time_begin = 0;
	threadFile1 = threadFiles[0];
	
	if (threadFiles.size() == 2) {
		threadFile2 = threadFiles[1];
	}
	else if (threadFiles.size() < 2) {
		throw std::runtime_error("Expected second input file for mixing");
	}
	else {
		throw std::invalid_argument("Extra arguments for mixing");
	}
	
	if (!parameters.empty()) {
		time_begin = parameters[0];
	}
	else {
		time_begin = 0;
	}


	Thread thread1(std::make_shared<std::string>(threadFile1));
	inputThread inputThread1(threadFile1, thread1);
	try {
		inputThread1.input();
	}
	catch (std::runtime_error const& ex) {
		throw ex;
	}

	Thread thread2(std::make_shared<std::string>(threadFile2));
	inputThread inputThread2(threadFile2, thread2);
	try {
		inputThread2.input();
	}
	catch (std::runtime_error const& ex) {
		throw ex;
	}


	size_t data_size = thread1.getNumberOfSamples();

	size_t begin = time_begin * thread1.getHeader().get_sample_rate();
	if (begin > data_size) {
		throw std::runtime_error("Unavailable argument of begin_time for mixing");
	}
	size_t end = (thread1.getNumberOfSamples() - begin <= thread2.getNumberOfSamples())
		? thread1.getNumberOfSamples() : begin + thread2.getNumberOfSamples();
	if (end > data_size) {
		throw std::runtime_error("Unavailable argument of duration for mixing");
	}


	FILE* fin1;
	fopen_s(&fin1, (*thread1.getFile()).c_str(), "rb");
	if (!fin1) {
		throw std::runtime_error("Unavailable input file for mixing");
	}

	FILE* fin2;
	fopen_s(&fin2, (*thread2.getFile()).c_str(), "rb");
	if (!fin2) {
		throw std::runtime_error("Unavailable input file for mixing");
	}


	readBuffer readBuff1(BUFF_SIZE, fin1, thread1.getData());
	readBuffer readBuff2(BUFF_SIZE, fin2, thread2.getData());


	Thread newThread;
	if (outputFile == nullptr) {
		std::string newFile = "mixed_" + (*thread1.getFile()) + '_' + (*thread2.getFile());
		newThread.setFile(std::make_shared<std::string>(newFile));
	}
	else {
		newThread.setFile(outputFile);
	}
	
	newThread.setHeader(thread1.getHeader());
	newThread.setData(thread1.getData());


	FILE* fout;
	fopen_s(&fout, (*newThread.getFile()).c_str(), "wb");
	if (!fout) {
		throw std::runtime_error("Unavailable output file for mixing");
	}

	outputHeader outputHeader(fout, newThread.getHeader());
	try {
		outputHeader.output();
	}
	catch (std::runtime_error const& ex) {
		throw ex;
	}

	writeBuffer writeBuff(BUFF_SIZE, fout, newThread.getData());


	//before begin
	for (size_t i = 0; i < begin; ++i) {
		writeBuff >> readBuff1[i];
	}

	//changing
	for (size_t i1 = begin, i2 = 0; i1 < end; ++i1, ++i2) {

		if (static_cast<int>(readBuff1[i1] + readBuff2[i2]) > SHRT_MAX) {
			writeBuff >> SHRT_MAX;
		}
		else {
			writeBuff >> readBuff1[i1] + readBuff2[i2];
		}
	}

	//after end
	for (size_t i = end; i < data_size; ++i) {
		writeBuff >> readBuff1[i];
	}

	fclose(fin1);
	fclose(fin2);
	fclose(fout);
	return newThread;
}