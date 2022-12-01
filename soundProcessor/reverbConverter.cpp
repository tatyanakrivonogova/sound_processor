#include "Factory.h"
#include "reverbConverter.h"
#include "Converter.h"
#include "Factory.h"
#include "Stream.h"
#include "readBuffer.h"
#include "writeBuffer.h"
#include "outputHeader.h"
#include "inputStream.h"
#include <fstream>
#include <iostream>
#include <string>
#define BUFF_SIZE 1000

extern AbstractFactory<Converter, std::string> ConverterFactory;

namespace {
	Converter* createReverbConverter() {
		return new reverbConverter();
	}

	const bool registered = ConverterFactory.Register("reverb", createReverbConverter);
}

void reverbConverter::whatAreYouDoing(FILE* fout) {
	std::string info;
	info += "reverbConverter\n";
	info += "\tcommand: reverb [start] duration delay intensity\n";
	info += "\treverbing of stream from 'start' to 'start+duration' seconds with delay of 'delay' and intensity of 'intensity' percent (default [start = 0])\n";
	fseek(fout, 0, SEEK_END);
	fwrite(info.data(), sizeof(char), info.size(), fout);
}

Stream reverbConverter::convert(std::vector<std::string> streamFiles, std::vector<double> parameters, std::shared_ptr<std::string> outputFile = nullptr) {

	std::string streamFile;
	double time_begin = 0;
	double duration;
	double delay;
	double intensity;

	streamFile = streamFiles[0];
	if (parameters.size() == 4) {
		time_begin = parameters[0];
		duration = parameters[1];
		delay = parameters[2];
		intensity = double(parameters[3]) / 100;
	}
	else if (parameters.size() == 3) {
		time_begin = 0;
		duration = parameters[0];
		delay = parameters[1];
		intensity = double(parameters[2]) / 100;
	}
	else if (parameters.size() < 3) {
		throw std::runtime_error("Expected reverbing duration, delay and intensity");
	}
	else {
		throw std::invalid_argument("Extra arguments for reverbing");
	}


	Stream stream(std::make_shared<std::string>(streamFile));
	inputStream inputStream1(streamFile, stream);
	try {
		inputStream1.input();
	}
	catch (std::runtime_error const& ex) {
		throw ex;
	}

	FILE* fin;
	fopen_s(&fin, (*stream.getFile()).c_str(), "rb");
	if (!fin) {
		throw std::runtime_error("Unavailable input file for reverbing");
	}

	readBuffer readBuff(BUFF_SIZE, fin, stream.getData());


	Stream newStream(stream);
	if (outputFile == nullptr) {
		std::string newFile = "reverbed_" + (*stream.getFile());
		newStream.setFile(std::make_shared<std::string>(newFile));
	}
	else {
		newStream.setFile(outputFile);
	}

	FILE* fout;
	fopen_s(&fout, (*newStream.getFile()).c_str(), "wb");
	if (!fout) {
		throw std::runtime_error("Unavailable output file for reverbing");
	}

	outputHeader outputHeader(fout, newStream.getHeader());
	try {
		outputHeader.output();
	}
	catch (std::runtime_error const& ex) {
		throw ex;
	}


	writeBuffer writeBuff(BUFF_SIZE, fout, newStream.getData());

	size_t data_size = (stream.getHeader().get_chunk_size() - stream.getData()) / 2;
	size_t begin = static_cast<size_t>(time_begin * stream.getHeader().get_sample_rate());
	if (begin > data_size) {
		throw std::runtime_error("Unavailable argument of begin_time for reverbing");
	}

	size_t end = static_cast<size_t>((time_begin + duration) * stream.getHeader().get_sample_rate());
	if (end > data_size) {
		throw std::runtime_error("Unavailable argument of duration for reverbing");
	}
	if (begin > end) {
		throw std::runtime_error("Unavailable argument of begin_time for reverbing");
	}
	if (delay > data_size) {
		throw std::runtime_error("Unavailable argument of delay for reverbing");
	}

	//before begin
	for (size_t i = 0; i < begin; ++i) {
		writeBuff >> readBuff[i];
	}

	//changing
	delay *= newStream.getHeader().get_sample_rate();
	for (size_t i = begin; i < end; ++i) {
		int sample = (i > delay) ? (static_cast<int>(readBuff[i]) + static_cast<int>(short(intensity*readBuff[i - static_cast<size_t>(delay)]))) : static_cast<int>(readBuff[i]);
		if (sample > SHRT_MAX) {
			writeBuff >> SHRT_MAX;
		}
		else if (sample < SHRT_MIN) {
			writeBuff >> SHRT_MIN;
		}
		else {
			writeBuff >> static_cast<short>(sample);
		}
	}

	//after end
	for (size_t i = end; i < data_size; ++i) {
		writeBuff >> readBuff[i];
	}

	fclose(fin);
	fclose(fout);
	return newStream;
}