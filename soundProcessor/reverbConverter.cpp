#include "Factory.h"
#include "reverbConverter.h"
#include "Converter.h"
#include "Factory.h"
#include "Stream.h"
#include "readBuffer.h"
#include "writeBuffer.h"
#include "outputHeader.h"
#include "inputStream.h"
#include "sumStreams.h"
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

void reverbConverter::setArgs(std::vector<std::string>& streamFiles, std::vector<double>& parameters, 
	std::string& streamFile, double& time_begin, double& duration, double& delay, double& intensity) {

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
}

void reverbConverter::checkArgs(Stream& stream, size_t& data_size, size_t& begin, size_t& end, 
	double& time_begin, double& duration, double& delay, double& intensity) {

	data_size = (stream.getHeader().get_chunk_size() - stream.getData()) / 2;
	begin = static_cast<size_t>(time_begin * stream.getHeader().get_sample_rate());
	end = static_cast<size_t>((time_begin + duration) * stream.getHeader().get_sample_rate());
	delay = static_cast<size_t>((delay) * stream.getHeader().get_sample_rate());
	if (begin > data_size) {
		throw std::runtime_error("Unavailable argument of begin_time for reverbing");
	}


	if (end > data_size) {
		throw std::runtime_error("Unavailable argument of duration for reverbing");
	}
	if (begin > end) {
		throw std::runtime_error("Unavailable argument of begin_time for reverbing");
	}
	if (delay > data_size) {
		throw std::runtime_error("Unavailable argument of delay for reverbing");
	}

}

void reverbConverter::prepareStreams(std::shared_ptr<std::string> outputFile, 
	std::string& streamFile, Stream& stream, Stream& newStream, double& delay) {
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
		throw std::runtime_error("Unavailable input file for reverbing");
	}

	if (outputFile == nullptr) {
		std::string newFile = "reverbed_" + (*stream.getFile());
		newStream.setFile(std::make_shared<std::string>(newFile));
	}
	else {
		newStream.setFile(outputFile);
	}

	fopen_s(&fout, (*newStream.getFile()).c_str(), "wb");
	if (!fout) {
		throw std::runtime_error("Unavailable output file for reverbing");
	}

	newStream.getHeader().get_subchunk3_size() = static_cast<size_t>(delay + newStream.getHeader().get_subchunk3_size());
	outputHeader outputHeader(fout, newStream.getHeader());

	try {
		outputHeader.output();
	}
	catch (std::runtime_error const& ex) {
		throw ex;
	}
}

Stream reverbConverter::convert(std::vector<std::string>& streamFiles, std::vector<double>& parameters, std::shared_ptr<std::string> outputFile = nullptr) {

	std::string streamFile;
	double time_begin = 0, duration, delay, intensity;

	size_t data_size, begin, end;

	setArgs(streamFiles, parameters, streamFile, time_begin, duration, delay, intensity);
	Stream stream(std::make_shared<std::string>(streamFile));
	Stream newStream;

	prepareStreams(outputFile, streamFile, stream, newStream, delay);
	checkArgs(stream, data_size, begin, end, time_begin, duration, delay, intensity);


	readBuffer readBuff(BUFF_SIZE, fin, stream.getData());
	writeBuffer writeBuff(BUFF_SIZE, fout, newStream.getData());

	//before begin
	for (size_t i = 0; i < begin+delay; ++i) {
		writeBuff >> readBuff[i];
	}

	//changing
	//delay *= newStream.getHeader().get_sample_rate();
	//end += static_cast<size_t>(delay);
	sumStreams(readBuff, readBuff, writeBuff, stream.getHeader().get_subchunk3_size(), 
		newStream.getHeader().get_subchunk3_size(), static_cast<size_t>(begin+delay), begin, static_cast<size_t>(end + delay), 1.0, intensity);

	//after end
	for (size_t i = end; i < data_size; ++i) {
		writeBuff >> readBuff[i];
	}

	fclose(fin);
	fclose(fout);
	return newStream;
}