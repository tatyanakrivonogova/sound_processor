#include "muteConverter.h"
#include "Converter.h"
#include "Thread.h"
#include <fstream>
#include <iostream>
#define BUF_SIZE 1000


Thread muteConverter::convert() {
	FILE* fin;
	fopen_s(&fin, (*thread.getFile()).c_str(), "rb");


	Thread newThread(thread);
	std::string newFile = "new_" + (*thread.getFile());
	newThread.setFile(std::make_shared<std::string>(newFile));


	FILE* fout;
	fopen_s(&fout, (*newThread.getFile()).c_str(), "wb");



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



	fseek(fin, thread.getData(), std::ios::beg);


	short* buffer = new short[BUF_SIZE];
	size_t pos = 0;
	size_t begin = time_begin * 44100;
	size_t end = (time_begin + duration) * 44100;
	size_t data_size = thread.getHeader().get_subchunk3_size();


	//before begin
	while (pos + BUF_SIZE < begin) {
		fread(buffer, 2, BUF_SIZE, fin);
		fwrite(buffer, 2, BUF_SIZE, fout);
		pos += BUF_SIZE;
	}

	fread(buffer, 2, begin - pos, fin);
	fwrite(buffer, 2, begin - pos, fout);

	pos = begin;


	//changing
	for (size_t i = 0; i < BUF_SIZE; ++i) {
		buffer[i] = 0;
	}
	while (pos + BUF_SIZE < end) {
		fwrite(buffer, 2, BUF_SIZE, fout);
		pos += BUF_SIZE;
	}

	fwrite(buffer, 2, end - pos, fout);
	pos = end;

	//after end
	while (pos + BUF_SIZE < data_size) {
		fread(buffer, 2, BUF_SIZE, fin);
		fwrite(buffer, 2, BUF_SIZE, fout);
		pos += BUF_SIZE;
	}

	fread(buffer, 2, data_size - pos, fin);
	fwrite(buffer, 2, data_size - pos, fout);

	return newThread;
}