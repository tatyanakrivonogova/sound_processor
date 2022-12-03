#include "Factory.h"
#include "muteConverter.h"
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
	Converter* createMuteConverter() {
		return new muteConverter();
	}
	
	const bool registered = ConverterFactory.Register("mute", createMuteConverter);
}

void muteConverter::whatAreYouDoing(FILE* fout) {
	std::string info;
	info += "muteConverter\n";
	info += "\tcommand: mute [start] duration\n";
	info += "\tmuting of stream from 'start' to 'start+duration' seconds (default [start = 0])\n";
	fseek(fout, 0, SEEK_END);
	fwrite(info.data(), sizeof(char), info.size(), fout);
}

void muteConverter::setArgs(std::vector<std::string>& streamFiles, std::vector<double>& parameters,
	std::string& streamFile, double& time_begin, double& duration) {

	streamFile = streamFiles[0];
	if (parameters.size() == 2) {
		time_begin = parameters[0];
		duration = parameters[1];
	}
	else if (parameters.size() == 1) {
		time_begin = 0;
		duration = parameters[0];
	}
	else if (parameters.size() == 0) {
		throw std::runtime_error("Expected muting duration for muting");
	}
	else {
		throw std::invalid_argument("Extra arguments for muting");
	}
}

void muteConverter::checkArgs(Stream& stream, size_t& data_size, size_t& begin, size_t& end,
	double& time_begin, double& duration) {

	data_size = (stream.getHeader().get_chunk_size() - stream.getData()) / 2;
	begin = static_cast<size_t>(time_begin * stream.getHeader().get_sample_rate());
	if (begin > data_size) {
		throw std::runtime_error("Unavailable argument of begin_time for muting");
	}

	end = static_cast<size_t>((time_begin + duration) * stream.getHeader().get_sample_rate());
	if (end > data_size) {
		throw std::runtime_error("Unavailable argument of duration for muting");
	}
	if (begin > end) {
		throw std::runtime_error("Unavailable argument of begin_time for muting");
	}

}

void muteConverter::prepareStreams(std::shared_ptr<std::string> outputFile,
	std::string& streamFile, Stream& stream, Stream& newStream) {
	inputStream inputStream1(streamFile, stream);
	try {
		inputStream1.input();
	}
	catch (std::runtime_error const& ex) {
		throw ex;
	}

	fopen_s(&fin, (*stream.getFile()).c_str(), "rb");
	if (!fin) {
		throw std::runtime_error("Unavailable input file for muting");
	}

	newStream = stream;
	if (outputFile == nullptr) {
		std::string newFile = "muted_" + (*stream.getFile());
		newStream.setFile(std::make_shared<std::string>(newFile));
	}
	else {
		newStream.setFile(outputFile);
	}

	fopen_s(&fout, (*newStream.getFile()).c_str(), "wb");
	if (!fout) {
		throw std::runtime_error("Unavailable output file for muting");
	}

	outputHeader outputHeader(fout, newStream.getHeader());
	try {
		outputHeader.output();
	}
	catch (std::runtime_error const& ex) {
		throw ex;
	}
}

Stream muteConverter::convert(std::vector<std::string>& streamFiles, std::vector<double>& parameters, std::shared_ptr<std::string> outputFile = nullptr) {

	std::string streamFile;
	double time_begin = 0, duration;
	size_t data_size, begin, end;

	setArgs(streamFiles, parameters, streamFile, time_begin, duration);
	Stream stream(std::make_shared<std::string>(streamFile));
	Stream newStream;

	prepareStreams(outputFile, streamFile, stream, newStream);
	checkArgs(stream, data_size, begin, end, time_begin, duration);

	
	readBuffer readBuff(BUFF_SIZE, fin, stream.getData());
	writeBuffer writeBuff(BUFF_SIZE, fout, newStream.getData());

	//before begin
	for (size_t i = 0; i < begin; ++i) {
		writeBuff >> readBuff[i];
	}

	//changing
	for (size_t i = begin; i < end; ++i) {
		writeBuff >> 0;
	}

	//after end
	for (size_t i = end; i < data_size; ++i) {
		writeBuff >> readBuff[i];
	}

	fclose(fin);
	fclose(fout);
	return newStream;
}