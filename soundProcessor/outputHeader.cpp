#include "outputHeader.h"
#include <iostream>

void outputHeader::output() {

	fwrite(header.get_chunk_ID(), 1, 4, fout);
	fwrite(&header.get_chunk_size(), 4, 1, fout);
	fwrite(header.get_format(), 1, 4, fout);


	fwrite(header.get_subchunk1_ID(), 1, 4, fout);
	fwrite(&header.get_subchunk1_size(), 4, 1, fout);

	char buffer2[2];
	char buffer4[4];

	buffer2[0] = char(header.get_audio_format() & 0xff);
	buffer2[1] = char((header.get_audio_format() >> 8) & 0xff);
	fwrite(buffer2, 1, 2, fout);


	buffer2[0] = char(header.get_num_channels() & 0xff);
	buffer2[1] = char((header.get_num_channels() >> 8) & 0xff);
	fwrite(buffer2, 1, 2, fout);


	buffer4[0] = char(header.get_sample_rate() & 0xff);
	buffer4[1] = char((header.get_sample_rate() >> 8) & 0xff);
	buffer4[2] = char((header.get_sample_rate() >> 16) & 0xff);
	buffer4[3] = char((header.get_sample_rate() >> 24) & 0xff);
	fwrite(buffer4, 1, 4, fout);


	buffer4[0] = unsigned char(header.get_byte_rate() & 0xff);
	buffer4[1] = unsigned char((header.get_byte_rate() >> 8) & 0xff);
	buffer4[2] = unsigned char((header.get_byte_rate() >> 16) & 0xff);
	buffer4[3] = unsigned char((header.get_byte_rate() >> 24) & 0xff);
	fwrite(buffer4, 1, 4, fout);


	buffer2[0] = char(header.get_block_align() & 0xff);
	buffer2[1] = char((header.get_block_align() >> 8) & 0xff);
	fwrite(buffer2, 1, 2, fout);


	buffer2[0] = char(header.get_bits_per_sample() & 0xff);
	buffer2[1] = char((header.get_bits_per_sample() >> 8) & 0xff);
	fwrite(buffer2, 1, 2, fout);

	fwrite(header.get_subchunk3_ID(), 1, 4, fout);
	fwrite(&header.get_subchunk3_size(), 4, 1, fout);

}

	//fwrite(thread.getHeader().get_chunk_ID(), 1, 4, fout);
	//fwrite(&thread.getHeader().get_chunk_size(), 4, 1, fout);
	//fwrite(thread.getHeader().get_format(), 1, 4, fout);


	//fwrite(thread.getHeader().get_subchunk1_ID(), 1, 4, fout);
	//fwrite(&thread.getHeader().get_subchunk1_size(), 4, 1, fout);

	//char buffer2[2];
	//char buffer4[4];

	//buffer2[0] = char(thread.getHeader().get_audio_format() & 0xff);
	//buffer2[1] = char((thread.getHeader().get_audio_format() >> 8) & 0xff);
	//fwrite(buffer2, 1, 2, fout);


	//buffer2[0] = char(thread.getHeader().get_num_channels() & 0xff);
	//buffer2[1] = char((thread.getHeader().get_num_channels() >> 8) & 0xff);
	//fwrite(buffer2, 1, 2, fout);


	//buffer4[0] = char(thread.getHeader().get_sample_rate() & 0xff);
	//buffer4[1] = char((thread.getHeader().get_sample_rate() >> 8) & 0xff);
	//buffer4[2] = char((thread.getHeader().get_sample_rate() >> 16) & 0xff);
	//buffer4[3] = char((thread.getHeader().get_sample_rate() >> 24) & 0xff);
	//fwrite(buffer4, 1, 4, fout);


	//buffer4[0] = unsigned char(thread.getHeader().get_byte_rate() & 0xff);
	//buffer4[1] = unsigned char((thread.getHeader().get_byte_rate() >> 8) & 0xff);
	//buffer4[2] = unsigned char((thread.getHeader().get_byte_rate() >> 16) & 0xff);
	//buffer4[3] = unsigned char((thread.getHeader().get_byte_rate() >> 24) & 0xff);
	//fwrite(buffer4, 1, 4, fout);


	//buffer2[0] = char(thread.getHeader().get_block_align() & 0xff);
	//buffer2[1] = char((thread.getHeader().get_block_align() >> 8) & 0xff);
	//fwrite(buffer2, 1, 2, fout);


	//buffer2[0] = char(thread.getHeader().get_bits_per_sample() & 0xff);
	//buffer2[1] = char((thread.getHeader().get_bits_per_sample() >> 8) & 0xff);
	//fwrite(buffer2, 1, 2, fout);

	//fwrite(thread.getHeader().get_subchunk3_ID(), 1, 4, fout);
	//fwrite(&thread.getHeader().get_subchunk3_size(), 4, 1, fout);