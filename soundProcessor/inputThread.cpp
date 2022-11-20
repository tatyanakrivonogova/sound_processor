#include "inputThread.h"
#include "WAVheader.h"
#include "Thread.h"
#include <iostream>
#include <fstream>

void swap_pointers(char* a, char* b) {
	char* tmp = a;
	a = b;
	b = tmp;
}


int inputThread::input() {
	std::ifstream fin;
	//FILE* fin = fopen(inputFile.c_str(), "rb");
	fin.open(inputFile, std::ios::binary);

	if (!fin) {
		std::cout << "File is not available" << std::endl;
		return -1;
	}
	thread.setSource(std::make_shared<std::string>(inputFile));

	char* buffer4 = new char[4];
	char* buffer2 = new char[2];

	fin.read(thread.getHeader().get_chunk_ID(), 4);
	if (std::string(thread.getHeader().get_chunk_ID(), 4) != "RIFF") {
		std::cout << "Can not find chunk 'RIFF'" << std::endl;
		return -1;
	}


	fin.read((char*)(&thread.getHeader().get_chunk_size()), 4);
	std::cout << " file size " << thread.getHeader().get_chunk_size() << std::endl;
	unsigned int rest_bytes = thread.getHeader().get_chunk_size();

	fin.read((char*)thread.getHeader().get_format(), 4);
	rest_bytes -= 4;
	if (std::string((char*)thread.getHeader().get_format(), 4) != "WAVE") {
		std::cout << "Can not find chunk 'WAVE'" << std::endl;
		return -1;
	}

	fin.read((char*)thread.getHeader().get_subchunk1_ID(), 4);
	rest_bytes -= 4;
	if (std::string((char*)thread.getHeader().get_subchunk1_ID(), 4) != "fmt ") {
		std::cout << "Can not find chunk 'fmt '" << std::endl;
		return -1;
	}

	fin.read((char*)(&thread.getHeader().get_subchunk1_size()), 4);
	rest_bytes -= 4;
	std::cout << " fmt size " << thread.getHeader().get_subchunk1_size() << std::endl;
	if (thread.getHeader().get_subchunk1_size() == 0) {
		std::cout << "Can not find size of audio-information" << std::endl;
		return -1;
	}


	fin.read((char*)buffer2, 2);
	thread.getHeader().get_audio_format() = (buffer2[0] & 0xff) | ((buffer2[1] & 0xff) << 8);
	std::cout << " audio_format " << thread.getHeader().get_audio_format() << std::endl;
	rest_bytes -= 2;


	fin.read((char*)buffer2, 2);
	thread.getHeader().get_num_channels() = (buffer2[0] & 0xff) | ((buffer2[1] & 0xff) << 8);
	std::cout << " num_channels " << thread.getHeader().get_num_channels() << std::endl;
	rest_bytes -= 2;


	fin.read((char*)buffer4, 4);
	thread.getHeader().get_sample_rate() = (buffer4[0] & 0xff) | ((buffer4[1] & 0xff) << 8) | ((buffer4[2] & 0xff) << 16) | ((buffer4[3] & 0xff) << 24);
	std::cout << " sample_rate " << thread.getHeader().get_sample_rate() << std::endl;
	rest_bytes -= 4;


	fin.read((char*)buffer4, 4);
	
	thread.getHeader().get_byte_rate() = (buffer4[0] & 0xff) | ((buffer4[1] & 0xff) << 8) | ((buffer4[2] & 0xff) << 16) | ((buffer4[3] & 0xff) << 24);
	std::cout << " byte rate " << thread.getHeader().get_byte_rate() << std::endl;
	rest_bytes -= 4;


	fin.read((char*)buffer2, 2);
	thread.getHeader().get_block_align() = (buffer2[0] & 0xff) | ((buffer2[1] & 0xff) << 8);
	std::cout << " block align " << thread.getHeader().get_block_align() << std::endl;
	rest_bytes -= 2;


	fin.read((char*)buffer2, 2);
	thread.getHeader().get_bits_per_sample() = (buffer2[0] & 0xff) | ((buffer2[1] & 0xff) << 8);
	std::cout << " bits per sample " << thread.getHeader().get_bits_per_sample() << std::endl;
	rest_bytes -= 2;

	std::cout << " 2 ID " << fin.tellg() << std::endl;
	fin.read((char*)thread.getHeader().get_subchunk2_ID(), 4);
	rest_bytes -= 4;

	if (std::string((char*)thread.getHeader().get_subchunk2_ID(), 4) == "fact") {
		std::cout << "Find chunk 'fact'" << std::endl;


		fin.read((char*)(&thread.getHeader().get_subchunk2_size()), 4);
		rest_bytes -= 4;
		std::cout << " fact size " << thread.getHeader().get_subchunk2_size() << std::endl;
		fin.read((char*)thread.getHeader().get_subchunk2_data(), thread.getHeader().get_subchunk2_size());
		rest_bytes -= thread.getHeader().get_subchunk2_size();

		fin.read((char*)thread.getHeader().get_subchunk3_ID(), 4);
		rest_bytes -= 4;
	}
	else if (std::string((char*)thread.getHeader().get_subchunk2_ID(), 4) == "LIST") {
		std::cout << "Find chunk 'list'" << std::endl;


		fin.read((char*)(&thread.getHeader().get_subchunk2_size()), 4);
		rest_bytes -= 4;
		std::cout << " list size " << thread.getHeader().get_subchunk2_size() << std::endl;
		fin.read((char*)thread.getHeader().get_subchunk2_data(), thread.getHeader().get_subchunk2_size());
		rest_bytes -= thread.getHeader().get_subchunk2_size();

		std::cout << " 3 ID " << fin.tellg() << std::endl;
		fin.read((char*)thread.getHeader().get_subchunk3_ID(), 4);
		rest_bytes -= 4;
	}
	else {
		std::memcpy(thread.getHeader().get_subchunk3_ID(), thread.getHeader().get_subchunk2_ID(), sizeof(char) * 4);
	}

	if (std::string((char*)thread.getHeader().get_subchunk3_ID(), 4) != "data") {
		std::cout << "Can not find chunk 'data'" << std::endl;
		return -1;
	}

	fin.read((char*)(&thread.getHeader().get_subchunk3_size()), 4);
	rest_bytes -= 4;
	std::cout << " data size " << thread.getHeader().get_subchunk3_size() << std::endl;

	//char* data = new char[thread.getHeader().subchunk3_size];
	/*std::vector<char> data;
	data.resize(thread.getHeader().get_subchunk3_size());

	fin.read(&data[0], thread.getHeader().get_subchunk3_size());
	if (fin.fail()) {
		std::cout << "problem!!!" << std::endl;
	}*/
	//thread.setData(thread.getHeader().get_chunk_size()- thread.getHeader().get_subchunk3_size());
	thread.setData(fin.tellg());

	std::cout << "OK!" << std::endl;
	std::cout << rest_bytes << std::endl;
	fin.close();
	delete [] buffer4;
	delete [] buffer2;
	return 0;
}
