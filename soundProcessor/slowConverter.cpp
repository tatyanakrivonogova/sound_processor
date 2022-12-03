#include "Factory.h"
#include "slowConverter.h"
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
	Converter* createSlowConverter() {
		return new slowConverter();
	}

	const bool registered = ConverterFactory.Register("slow", createSlowConverter);
}


void slowConverter::whatAreYouDoing(FILE* fout) {
	std::string info;
	info += "slowConverter\n";
	info += "\tcommand: slow [start] duration ratio\n";
	info += "\tslowing of stream from 'start' to 'start+duration' seconds by 'ratio' times (default [start = 0])\n";
	fseek(fout, 0, SEEK_END);
	fwrite(info.data(), sizeof(char), info.size(), fout);
}

void slowConverter::setArgs(std::vector<std::string>& streamFiles, std::vector<double>& parameters,
	std::string& streamFile, double& time_begin, double& duration, double& ratio) {

	streamFile = streamFiles[0];
	if (parameters.size() == 3) {
		time_begin = parameters[0];
		duration = parameters[1];
		ratio = parameters[2];
	}
	else if (parameters.size() == 2) {
		time_begin = 0;
		duration = parameters[0];
		ratio = parameters[1];
	}
	else if (parameters.size() == 1) {
		throw std::runtime_error("Expected ratio of slowing down");
	}
	else if (parameters.size() == 0) {
		throw std::runtime_error("Expected slowing duration");
	}
	else {
		throw std::invalid_argument("Extra arguments for slowing");
	}
}

void slowConverter::checkArgs(Stream& stream, size_t& data_size, size_t& begin, size_t& end,
	double& time_begin, double& duration, double& ratio) {

	data_size = (stream.getHeader().get_chunk_size() - stream.getData()) / 2;
	begin = static_cast<size_t>(time_begin * stream.getHeader().get_sample_rate());
	if (begin > data_size) {
		throw std::runtime_error("Unavailable argument of begin_time for slowing");
	}

	end = static_cast<size_t>((time_begin + duration) * stream.getHeader().get_sample_rate());
	if (end > data_size) {
		throw std::runtime_error("Unavailable argument of duration for slowing");
	}
	if (begin > end) {
		throw std::runtime_error("Unavailable argument of begin_time for slowing");
	}

}

void slowConverter::prepareStreams(std::shared_ptr<std::string> outputFile,
	std::string& streamFile, Stream& stream, Stream& newStream, double& ratio) {
	inputStream inputStream1(streamFile, stream);
	try {
		inputStream1.input();
	}
	catch (std::runtime_error const& ex) {
		throw ex;
	}
	newStream = stream;

	fopen_s(&fin, (*stream.getFile()).c_str(), "rb");
	if (!fin) {
		throw std::runtime_error("Unavailable input file for slowing");
	}

	if (outputFile == nullptr) {
		std::string newFile = "slowed_" + (*stream.getFile());
		newStream.setFile(std::make_shared<std::string>(newFile));
	}
	else {
		newStream.setFile(outputFile);
	}

	fopen_s(&fout, (*newStream.getFile()).c_str(), "wb");
	if (!fout) {
		throw std::runtime_error("Unavailable output file for slowing");
	}

	newStream.getHeader().get_subchunk3_size() = static_cast<size_t>(ratio * newStream.getHeader().get_subchunk3_size());
	outputHeader outputHeader(fout, newStream.getHeader());
	try {
		outputHeader.output();
	}
	catch (std::runtime_error const& ex) {
		throw ex;
	}
}

Stream slowConverter::convert(std::vector<std::string>& streamFiles, std::vector<double>& parameters, std::shared_ptr<std::string> outputFile = nullptr) {

	std::string streamFile;
	double time_begin = 0, duration, ratio;

	size_t data_size, begin, end;

	setArgs(streamFiles, parameters, streamFile, time_begin, duration, ratio);
	Stream stream(std::make_shared<std::string>(streamFile));
	Stream newStream;

	prepareStreams(outputFile, streamFile, stream, newStream, ratio);
	checkArgs(stream, data_size, begin, end, time_begin, duration, ratio);


	readBuffer readBuff(BUFF_SIZE, fin, stream.getData());
	writeBuffer writeBuff(BUFF_SIZE, fout, newStream.getData());
	//before begin
	for (size_t i = 0; i < begin; ++i) {
		writeBuff >> readBuff[i];
	}

	//changing
	for (size_t i = begin; i < end; ++i) {
		for (size_t j = 0; j < ratio; ++j) {
			writeBuff >> readBuff[i];
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