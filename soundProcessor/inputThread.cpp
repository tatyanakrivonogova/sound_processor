#include "inputThread.h"
#include "WAVheader.h"
#include "Thread.h"
#include <iostream>
#include <fstream>
#include <cstdio>

void swap_pointers(char* a, char* b) {
	char* tmp = a;
	a = b;
	b = tmp;
}


int inputThread::input() {
	FILE* fin;
	fopen_s(&fin, inputFile.c_str(), "rb");
	//fopen(fin, inputFile.c_str(), "rb");

	if (!fin) {
		std::cout << "File is not available" << std::endl;
		return -1;
	}
	//thread.setFile(std::make_shared<std::string>(inputFile));

	char buffer2[2];
	char buffer4[4];

	fread(thread.getHeader().get_chunk_ID(), 1, 4, fin);
	if (std::string(thread.getHeader().get_chunk_ID(), 4) != "RIFF") {
		std::cout << "Can not find chunk 'RIFF'" << std::endl;
		return -1;
	}


	fread(&thread.getHeader().get_chunk_size(), 4, 1, fin);
	std::cout << " file size " << thread.getHeader().get_chunk_size() << std::endl;
	unsigned int rest_bytes = thread.getHeader().get_chunk_size();

	fread(thread.getHeader().get_format(), 1, 4, fin);
	rest_bytes -= 4;
	if (std::string((char*)thread.getHeader().get_format(), 4) != "WAVE") {
		std::cout << "Can not find chunk 'WAVE'" << std::endl;
		return -1;
	}

	fread(thread.getHeader().get_subchunk1_ID(), 1, 4, fin);
	rest_bytes -= 4;
	if (std::string((char*)thread.getHeader().get_subchunk1_ID(), 4) != "fmt ") {
		std::cout << "Can not find chunk 'fmt '" << std::endl;
		return -1;
	}

	fread(&thread.getHeader().get_subchunk1_size(), 4, 1, fin);
	rest_bytes -= 4;
	std::cout << " fmt size " << thread.getHeader().get_subchunk1_size() << std::endl;
	if (thread.getHeader().get_subchunk1_size() == 0) {
		std::cout << "Can not find size of audio-information" << std::endl;
		return -1;
	}


	fread(buffer2, 1, 2, fin);
	thread.getHeader().get_audio_format() = (buffer2[0] & 0xff) | ((buffer2[1] & 0xff) << 8);
	std::cout << " audio_format " << thread.getHeader().get_audio_format() << std::endl;
	rest_bytes -= 2;


	fread(buffer2, 1, 2, fin);
	thread.getHeader().get_num_channels() = (buffer2[0] & 0xff) | ((buffer2[1] & 0xff) << 8);
	std::cout << " num_channels " << thread.getHeader().get_num_channels() << std::endl;
	rest_bytes -= 2;


	fread(buffer4, 1, 4, fin);
	thread.getHeader().get_sample_rate() = (buffer4[0] & 0xff) | ((buffer4[1] & 0xff) << 8) | ((buffer4[2] & 0xff) << 16) | ((buffer4[3] & 0xff) << 24);
	std::cout << " sample_rate " << thread.getHeader().get_sample_rate() << std::endl;
	rest_bytes -= 4;


	fread(buffer4, 1, 4, fin);
	thread.getHeader().get_byte_rate() = (buffer4[0] & 0xff) | ((buffer4[1] & 0xff) << 8) | ((buffer4[2] & 0xff) << 16) | ((buffer4[3] & 0xff) << 24);
	std::cout << " byte rate " << thread.getHeader().get_byte_rate() << std::endl;
	rest_bytes -= 4;


	fread(buffer2, 1, 2, fin);
	thread.getHeader().get_block_align() = (buffer2[0] & 0xff) | ((buffer2[1] & 0xff) << 8);
	std::cout << " block align " << thread.getHeader().get_block_align() << std::endl;
	rest_bytes -= 2;


	fread(buffer2, 1, 2, fin);
	thread.getHeader().get_bits_per_sample() = (buffer2[0] & 0xff) | ((buffer2[1] & 0xff) << 8);
	std::cout << " bits per sample " << thread.getHeader().get_bits_per_sample() << std::endl;
	rest_bytes -= 2;

	std::cout << " 2 ID " << ftell(fin) << std::endl;
	fread(thread.getHeader().get_subchunk3_ID(), 1, 4, fin);
	rest_bytes -= 4;

	if (std::string((char*)thread.getHeader().get_subchunk3_ID(), 4) == "fact") {
		//std::cout << "Find chunk 'fact'" << std::endl;
		unsigned int fact_size;
		//fread(&thread.getHeader().get_subchunk2_size(), 4, 1, fin);
		fread(&fact_size, 4, 1, fin);
		rest_bytes -= 4;
		//std::cout << " fact size " << thread.getHeader().get_subchunk2_size() << std::endl;
		//fread(thread.getHeader().get_subchunk2_data(), 1, thread.getHeader().get_subchunk2_size(), fin);
		fseek(fin, fact_size, SEEK_CUR);
		//rest_bytes -= thread.getHeader().get_subchunk2_size();
		rest_bytes -= fact_size;

		fread(thread.getHeader().get_subchunk3_ID(), 1, 4, fin);
		rest_bytes -= 4;

	}
	else if (std::string((char*)thread.getHeader().get_subchunk3_ID(), 4) == "LIST") {
		//std::cout << "Find chunk 'list'" << std::endl;
		unsigned int list_size;

		//fread(&thread.getHeader().get_subchunk2_size(), 4, 1, fin);
		fread(&list_size, 4, 1, fin);
		rest_bytes -= 4;
		//std::cout << " list size " << thread.getHeader().get_subchunk2_size() << std::endl;
		//fread(thread.getHeader().get_subchunk2_data(), 1, thread.getHeader().get_subchunk2_size(), fin);
		fseek(fin, list_size, SEEK_CUR);
		rest_bytes -= list_size;

		//std::cout << " 3 ID " << ftell(fin) << std::endl;
		fread(thread.getHeader().get_subchunk3_ID(), 1, 4, fin);
		rest_bytes -= 4;
	}
	/*else {
		std::memcpy(thread.getHeader().get_subchunk3_ID(), thread.getHeader().get_subchunk2_ID(), sizeof(char) * 4);
	}*/

	if (std::string((char*)thread.getHeader().get_subchunk3_ID(), 4) != "data") {
		std::cout << "Can not find chunk 'data'" << std::endl;
		return -1;
	}

	fread(&thread.getHeader().get_subchunk3_size(), 4, 1, fin);
	rest_bytes -= 4;
	std::cout << " data size " << thread.getHeader().get_subchunk3_size() << std::endl;


	thread.setData(ftell(fin));
	thread.setNumberOfSamples(thread.getHeader().get_subchunk3_size() / 2);

	std::cout << "OK!" << std::endl;
	std::cout << rest_bytes << std::endl;
	fclose(fin);
	//delete [] buffer4;
	//delete [] buffer2;
	return 0;
}
