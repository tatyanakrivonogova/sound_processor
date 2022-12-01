#include "inputStream.h"
#include "WAVheader.h"
#include "Stream.h"
#include <iostream>
#include <fstream>
#include <cstdio>

void swap_pointers(char* a, char* b) {
	char* tmp = a;
	a = b;
	b = tmp;
}


int inputStream::input() {
	FILE* fin;
	fopen_s(&fin, inputFile.c_str(), "rb");
	if (!fin) {
		throw std::runtime_error("Unavailable input file");
	}

	char buffer2[2];
	char buffer4[4];

	fread(stream.getHeader().get_chunk_ID(), 1, 4, fin);
	if (std::string(stream.getHeader().get_chunk_ID(), 4) != "RIFF") {
		throw std::runtime_error("Unsupported file format. Chunk 'RIFF' was not found");
	}


	fread(&stream.getHeader().get_chunk_size(), 4, 1, fin);

	fread(stream.getHeader().get_format(), 1, 4, fin);
	if (std::string((char*)stream.getHeader().get_format(), 4) != "WAVE") {
		throw std::runtime_error("Unsupported file format. Chunk 'WAVE' was not found");
	}

	fread(stream.getHeader().get_subchunk1_ID(), 1, 4, fin);
	if (std::string((char*)stream.getHeader().get_subchunk1_ID(), 4) != "fmt ") {
		throw std::runtime_error("Unsupported file format. Chunk 'fmt' was not found");
	}

	fread(&stream.getHeader().get_subchunk1_size(), 4, 1, fin);

	fread(buffer2, 1, 2, fin);
	stream.getHeader().get_audio_format() = (buffer2[0] & 0xff) | ((buffer2[1] & 0xff) << 8);
	if (stream.getHeader().get_audio_format() != 1) {
		throw std::runtime_error("Unsupported file format. File is compressed");
	}


	fread(buffer2, 1, 2, fin);
	stream.getHeader().get_num_channels() = (buffer2[0] & 0xff) | ((buffer2[1] & 0xff) << 8);
	if (stream.getHeader().get_num_channels() != 1) {
		throw std::runtime_error("Unsupported file format. More than 1 channel");
	}


	fread(buffer4, 1, 4, fin);
	stream.getHeader().get_sample_rate() = (buffer4[0] & 0xff) | ((buffer4[1] & 0xff) << 8) | ((buffer4[2] & 0xff) << 16) | ((buffer4[3] & 0xff) << 24);
	if (stream.getHeader().get_sample_rate() != 44100) {
		throw std::runtime_error("Unsupported file format. Sample rate is not equal 44100 Hz");
	}


	fread(buffer4, 1, 4, fin);
	stream.getHeader().get_byte_rate() = (buffer4[0] & 0xff) | ((buffer4[1] & 0xff) << 8) | ((buffer4[2] & 0xff) << 16) | ((buffer4[3] & 0xff) << 24);


	fread(buffer2, 1, 2, fin);
	stream.getHeader().get_block_align() = (buffer2[0] & 0xff) | ((buffer2[1] & 0xff) << 8);


	fread(buffer2, 1, 2, fin);
	stream.getHeader().get_bits_per_sample() = (buffer2[0] & 0xff) | ((buffer2[1] & 0xff) << 8);
	if (stream.getHeader().get_bits_per_sample() != 16) {
		throw std::runtime_error("Unsupported file format. Bits per sample is not equal 16");
	}
	fread(stream.getHeader().get_subchunk3_ID(), 1, 4, fin);


	if (std::string((char*)stream.getHeader().get_subchunk3_ID(), 4) == "fact") {
		unsigned int fact_size;
		fread(&fact_size, 4, 1, fin);
		fseek(fin, fact_size, SEEK_CUR);
		fread(stream.getHeader().get_subchunk3_ID(), 1, 4, fin);
	}
	else if (std::string((char*)stream.getHeader().get_subchunk3_ID(), 4) == "LIST") {
		unsigned int list_size;
		fread(&list_size, 4, 1, fin);
		fseek(fin, list_size, SEEK_CUR);
		fread(stream.getHeader().get_subchunk3_ID(), 1, 4, fin);
	}

	if (std::string((char*)stream.getHeader().get_subchunk3_ID(), 4) != "data") {
		throw std::runtime_error("Unsupported file format. Chunk 'data' was not found");
	}

	fread(&stream.getHeader().get_subchunk3_size(), 4, 1, fin);


	stream.setData(ftell(fin));
	stream.setNumberOfSamples(stream.getHeader().get_subchunk3_size() / 2);

	fclose(fin);
	return 0;
}