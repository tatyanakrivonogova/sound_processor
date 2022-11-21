#include "muteConverter.h"
#include "Converter.h"
#include "Thread.h"
#include "readBuffer.h"
#include "writeBuffer.h"
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
	fwrite(thread.getHeader().get_chunk_ID(), 1, 4, fout);
	fwrite(&thread.getHeader().get_chunk_size(), 4, 1, fout);
	fwrite(thread.getHeader().get_format(), 1, 4, fout);


	fwrite(thread.getHeader().get_subchunk1_ID(), 1, 4, fout);
	fwrite(&thread.getHeader().get_subchunk1_size(), 4, 1, fout);

	char buffer2[2];
	char buffer4[4];

	buffer2[0] = char(thread.getHeader().get_audio_format() & 0xff);
	buffer2[1] = char((thread.getHeader().get_audio_format() >> 8) & 0xff);
	fwrite(buffer2, 1, 2, fout);


	buffer2[0] = char(thread.getHeader().get_num_channels() & 0xff);
	buffer2[1] = char((thread.getHeader().get_num_channels() >> 8) & 0xff);
	fwrite(buffer2, 1, 2, fout);


	buffer4[0] = char(thread.getHeader().get_sample_rate() & 0xff);
	buffer4[1] = char((thread.getHeader().get_sample_rate() >> 8) & 0xff);
	buffer4[2] = char((thread.getHeader().get_sample_rate() >> 16) & 0xff);
	buffer4[3] = char((thread.getHeader().get_sample_rate() >> 24) & 0xff);
	fwrite(buffer4, 1, 4, fout);


	buffer4[0] = unsigned char(thread.getHeader().get_byte_rate() & 0xff);
	buffer4[1] = unsigned char((thread.getHeader().get_byte_rate() >> 8) & 0xff);
	buffer4[2] = unsigned char((thread.getHeader().get_byte_rate() >> 16) & 0xff);
	buffer4[3] = unsigned char((thread.getHeader().get_byte_rate() >> 24) & 0xff);
	fwrite(buffer4, 1, 4, fout);


	buffer2[0] = char(thread.getHeader().get_block_align() & 0xff);
	buffer2[1] = char((thread.getHeader().get_block_align() >> 8) & 0xff);
	fwrite(buffer2, 1, 2, fout);


	buffer2[0] = char(thread.getHeader().get_bits_per_sample() & 0xff);
	buffer2[1] = char((thread.getHeader().get_bits_per_sample() >> 8) & 0xff);
	fwrite(buffer2, 1, 2, fout);

	fwrite(thread.getHeader().get_subchunk3_ID(), 1, 4, fout);
	fwrite(&thread.getHeader().get_subchunk3_size(), 4, 1, fout);

	writeBuffer writeBuf(BUFF_SIZE, fout, thread.getData());
	

	//short* buffer = new short[BUFF_SIZE];
	//size_t pos = 0;
	size_t begin = time_begin * 44100;
	size_t end = (time_begin + duration) * 44100;
	//size_t data_size = thread.getHeader().get_subchunk3_size();
	size_t data_size = thread.getHeader().get_chunk_size() - thread.getData();
	std::cout << " size: " << thread.getHeader().get_chunk_size() << std::endl;
	std::cout << " size3: " << thread.getHeader().get_subchunk3_size() << std::endl;


	////before begin
	//while (pos + BUFF_SIZE < begin) {
	//	fread(buffer, 2, BUFF_SIZE, fin);
	//	fwrite(buffer, 2, BUFF_SIZE, fout);
	//	pos += BUFF_SIZE;
	//}

	//fread(buffer, 2, begin - pos, fin);
	//fwrite(buffer, 2, begin - pos, fout);

	//pos = begin;


	////changing
	//for (size_t i = 0; i < BUFF_SIZE; ++i) {
	//	buffer[i] = 0;
	//}
	//while (pos + BUFF_SIZE < end) {
	//	fwrite(buffer, 2, BUFF_SIZE, fout);
	//	pos += BUFF_SIZE;
	//}

	//fwrite(buffer, 2, end - pos, fout);
	//pos = end;

	////after end
	//while (pos + BUFF_SIZE < data_size) {
	//	fread(buffer, 2, BUFF_SIZE, fin);
	//	fwrite(buffer, 2, BUFF_SIZE, fout);
	//	pos += BUFF_SIZE;
	//}

	//fread(buffer, 2, data_size - pos, fin);
	//fwrite(buffer, 2, data_size - pos, fout);

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