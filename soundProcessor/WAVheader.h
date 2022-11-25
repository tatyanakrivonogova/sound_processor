#pragma once

class WAVheader {
private:
	char chunk_ID[4];
	unsigned int chunk_size;
	char format[4];

	char subchunk1_ID[4];
	unsigned int subchunk1_size;
	unsigned int audio_format;
	unsigned int num_channels;
	unsigned int sample_rate;
	unsigned int byte_rate;
	unsigned int block_align;
	unsigned int bits_per_sample;

	//char subchunk2_ID[4];
	//unsigned int subchunk2_size;
	//char subchunk2_data[500];

	char subchunk3_ID[4];
	unsigned int subchunk3_size;

public:
	WAVheader() {}
	~WAVheader() {}

	WAVheader(const WAVheader&);
	WAVheader(WAVheader&&) noexcept;

	WAVheader& operator=(const WAVheader&);
	WAVheader& operator=(WAVheader&&) noexcept;

	unsigned int& get_chunk_size();
	unsigned int& get_subchunk1_size();
	unsigned int& get_audio_format();
	unsigned int& get_num_channels();
	unsigned int& get_sample_rate();
	unsigned int& get_byte_rate();
	unsigned int& get_block_align();
	unsigned int& get_bits_per_sample();
	//unsigned int& get_subchunk2_size();
	unsigned int& get_subchunk3_size();

	char* get_chunk_ID();
	char* get_format();
	char* get_subchunk1_ID();
	//char* get_subchunk2_ID();
	//char* get_subchunk2_data();
	char* get_subchunk3_ID();
};