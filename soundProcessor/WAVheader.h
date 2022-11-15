#pragma once

class WAVheader {
public:
	char chunk_ID[4];
	unsigned int chunk_size;
	char format[4];

	char subchunk1_ID[4];
	unsigned int subchunk1_size;
	unsigned int audio_format;
	unsigned int num_channels;
	unsigned long sample_rate;
	unsigned long byte_rate;
	unsigned int block_align;
	unsigned int bits_per_sample;

	char subchunk2_ID[4];
	unsigned int subchunk2_size;
	char subchunk2_data[250];

	char subchunk3_ID[4];
	unsigned int subchunk3_size;
};