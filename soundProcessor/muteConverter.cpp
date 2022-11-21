#include "muteConverter.h"
#include "Converter.h"
#include "Thread.h"
#include "readBuffer.h"
#include "writeBuffer.h"
#include "outputHeader.h"
#include <fstream>
#include <iostream>
#define BUFF_SIZE 1000


Thread muteConverter::convert() {
	FILE* fin;
	fopen_s(&fin, (*thread.getFile()).c_str(), "rb");

	fseek(fin, 0, SEEK_END);
	std::cout << "fin: " << ftell(fin) << std::endl;
	fseek(fin, 0, SEEK_SET);

	readBuffer readBuff(BUFF_SIZE, fin, thread.getData());


	Thread newThread(thread);
	std::string newFile = "new_" + (*thread.getFile());
	newThread.setFile(std::make_shared<std::string>(newFile));

	FILE* fout;
	fopen_s(&fout, (*newThread.getFile()).c_str(), "wb");


	//output header, create ceparate class
	outputHeader outputHeader(fout, thread.getHeader());
	outputHeader.output();

	

	writeBuffer writeBuf(BUFF_SIZE, fout, thread.getData());
	

	size_t begin = time_begin * 44100;
	size_t end = (time_begin + duration) * 44100;
	//size_t data_size = thread.getHeader().get_subchunk3_size();
	size_t data_size = thread.getHeader().get_chunk_size() - thread.getData();
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
	for (size_t i = end; i < data_size/2; ++i) {
		writeBuf >> readBuff[i];
	}

	return newThread;
}