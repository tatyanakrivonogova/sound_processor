#include "Factory.h"
#include "mixConverter.h"
#include "Converter.h"
#include "Factory.h"
#include "Thread.h"
#include "readBuffer.h"
#include "writeBuffer.h"
#include "outputHeader.h"
#include <fstream>
#include <iostream>
#define BUFF_SIZE 1000

extern AbstractFactory<Converter, std::string> ConverterFactory;

namespace {
	Converter* createMixConverter(std::vector<Thread> threads, std::vector<unsigned int> time_args) {
		return new mixConverter(threads, time_args);
	}

	const bool registered = ConverterFactory.Register("mix", createMixConverter);
}


Thread mixConverter::convert() {
	FILE* fin1;
	fopen_s(&fin1, (*thread1.getFile()).c_str(), "rb");

	FILE* fin2;
	fopen_s(&fin2, (*thread2.getFile()).c_str(), "rb");

	//fseek(fin, 0, SEEK_END);
	//std::cout << "fin: " << ftell(fin) << std::endl;
	//fseek(fin, 0, SEEK_SET);

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


	size_t begin = time_begin * 44100;
	//size_t end = (time_begin + duration) * 44100;
	size_t end = (thread1.getHeader().get_subchunk3_size() - begin <= thread2.getHeader().get_subchunk3_size())
		? thread1.getHeader().get_subchunk3_size() : begin + thread2.getHeader().get_subchunk3_size();
	size_t data_size = thread1.getHeader().get_subchunk3_size() / 2; //1 sample = 2 bit
	//size_t data_size = thread.getHeader().get_chunk_size() - thread.getData();


	//before begin
	for (size_t i = 0; i < begin; ++i) {
		writeBuf >> readBuff1[i];
	}

	//changing
	for (size_t i1 = begin, i2 = 0; i1 < end; ++i1, ++i2) {
		writeBuf >> readBuff1[i1] + readBuff2[i2];
	}

	//after end
	for (size_t i = end; i < data_size; ++i) {
		writeBuf >> readBuff1[i];
	}

	return newThread;
}