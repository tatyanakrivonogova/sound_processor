#include "outputThread.h"
#include "WAVheader.h"
#include "Thread.h"
#include <iostream>
#include <iosfwd>
#define BUF_SIZE 1000

int outputThread::output() {
	FILE* fout;
	fopen_s(&fout, "new_Ring08.wav", "wb");

	if (fout == 0) {
		std::cout << "No output file" << std::endl;
		return -1;
	}

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


	FILE* fin;
	fopen_s(&fin, (*thread.getFile()).c_str(), "rb");
	if (fin == 0) {
		std::cout << "No input file" << std::endl;
		return -1;
	}

	fseek(fin, thread.getData(), std::ios::beg);
	short* buffer = new short[BUF_SIZE];

	for (unsigned int i = 0; i < thread.getHeader().get_subchunk3_size() / BUF_SIZE; ++i) {

		fread(buffer, 2, BUF_SIZE, fin);
		fwrite(buffer, 2, BUF_SIZE, fout);
	}

	fread(buffer, 2, thread.getHeader().get_subchunk3_size() % BUF_SIZE, fin);
	fwrite(buffer, 2, thread.getHeader().get_subchunk3_size() % BUF_SIZE, fout);

	delete[] buffer;
	fclose(fout);
	fclose(fin);
	return 0;
}