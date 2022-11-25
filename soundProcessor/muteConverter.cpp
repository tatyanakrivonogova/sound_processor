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


Thread muteConverter::convert() {

	Thread thread(std::make_shared<std::string>(threadFile));
	inputThread inputThread1(threadFile, thread);
	inputThread1.input();

	FILE* fin;
	fopen_s(&fin, (*thread.getFile()).c_str(), "rb");

	//fseek(fin, 0, SEEK_END);
	//std::cout << "fin: " << ftell(fin) << std::endl;
	//fseek(fin, 0, SEEK_SET);

	readBuffer readBuff(BUFF_SIZE, fin, thread.getData());


	Thread newThread(thread);
	std::string newFile = "muted_" + (*thread.getFile());
	newThread.setFile(std::make_shared<std::string>(newFile));

	FILE* fout;
	fopen_s(&fout, (*newThread.getFile()).c_str(), "wb");


	outputHeader outputHeader(fout, newThread.getHeader());
	outputHeader.output();

	

	writeBuffer writeBuf(BUFF_SIZE, fout, newThread.getData());
	

	size_t begin = time_begin * 44100;
	size_t end = (time_begin + duration) * 44100;
	//size_t data_size = thread.getHeader().get_subchunk3_size();
	size_t data_size = (thread.getHeader().get_chunk_size() - thread.getData()) / 2;
	std::cout << " size: " << thread.getHeader().get_chunk_size() << std::endl;
	std::cout << " size3: " << thread.getHeader().get_subchunk3_size() << std::endl;


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