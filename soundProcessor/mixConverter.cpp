#include "Factory.h"
#include "mixConverter.h"
#include "Converter.h"
#include "Factory.h"
#include "Stream.h"
#include "readBuffer.h"
#include "writeBuffer.h"
#include "outputHeader.h"
#include "inputStream.h"
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
	info += "\tcommand: mix second stream [start]\n";
	info += "\tmixing of the main and second streams starting from 'start' second (default [start = 0])\n";
	fseek(fout, 0, SEEK_END);
	fwrite(info.data(), sizeof(char), info.size(), fout);
}

Stream mixConverter::convert(std::vector<std::string> streamFiles, std::vector<unsigned int> parameters, std::shared_ptr<std::string> outputFile = nullptr) {
	std::string streamFile1;
	std::string streamFile2;
	unsigned int time_begin = 0;
	streamFile1 = streamFiles[0];
	
	if (streamFiles.size() == 2) {
		streamFile2 = streamFiles[1];
	}
	else if (streamFiles.size() < 2) {
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


	Stream stream1(std::make_shared<std::string>(streamFile1));
	inputStream inputStream1(streamFile1, stream1);
	try {
		inputStream1.input();
	}
	catch (std::runtime_error const& ex) {
		throw ex;
	}

	Stream stream2(std::make_shared<std::string>(streamFile2));
	inputStream inputStream2(streamFile2, stream2);
	try {
		inputStream2.input();
	}
	catch (std::runtime_error const& ex) {
		throw ex;
	}


	size_t data_size = stream1.getNumberOfSamples();

	size_t begin = time_begin * stream1.getHeader().get_sample_rate();
	if (begin > data_size) {
		throw std::runtime_error("Unavailable argument of begin_time for mixing");
	}
	size_t end = (stream1.getNumberOfSamples() - begin <= stream2.getNumberOfSamples())
		? stream1.getNumberOfSamples() : begin + stream2.getNumberOfSamples();
	if (end > data_size) {
		throw std::runtime_error("Unavailable argument of duration for mixing");
	}


	FILE* fin1;
	fopen_s(&fin1, (*stream1.getFile()).c_str(), "rb");
	if (!fin1) {
		throw std::runtime_error("Unavailable input file for mixing");
	}

	FILE* fin2;
	fopen_s(&fin2, (*stream2.getFile()).c_str(), "rb");
	if (!fin2) {
		throw std::runtime_error("Unavailable input file for mixing");
	}


	readBuffer readBuff1(BUFF_SIZE, fin1, stream1.getData());
	readBuffer readBuff2(BUFF_SIZE, fin2, stream2.getData());


	Stream newStream;
	if (outputFile == nullptr) {
		std::string newFile = "mixed_" + (*stream1.getFile()) + '_' + (*stream2.getFile());
		newStream.setFile(std::make_shared<std::string>(newFile));
	}
	else {
		newStream.setFile(outputFile);
	}
	
	newStream.setHeader(stream1.getHeader());
	newStream.setData(stream1.getData());


	FILE* fout;
	fopen_s(&fout, (*newStream.getFile()).c_str(), "wb");
	if (!fout) {
		throw std::runtime_error("Unavailable output file for mixing");
	}

	outputHeader outputHeader(fout, newStream.getHeader());
	try {
		outputHeader.output();
	}
	catch (std::runtime_error const& ex) {
		throw ex;
	}

	writeBuffer writeBuff(BUFF_SIZE, fout, newStream.getData());


	//before begin
	for (size_t i = 0; i < begin; ++i) {
		writeBuff >> readBuff1[i];
	}

	//changing
	for (size_t i1 = begin, i2 = 0; i1 < end; ++i1, ++i2) {

		if (static_cast<int>(readBuff1[i1]) + static_cast<int>(readBuff2[i2]) > SHRT_MAX) {
			writeBuff >> SHRT_MAX;
		}
		else if (static_cast<int>(readBuff1[i1]) + static_cast<int>(readBuff2[i2]) < SHRT_MIN) {
			writeBuff >> SHRT_MIN;
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
	return newStream;
}