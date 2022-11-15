#include "outputThread.h"
#include "WAVheader.h"
#include "Thread.h"

int outputThread::output() {
	std::ofstream fout;
	fout.open("new_Ring08.wav", std::ios::binary);


	fout.write((char*)thread.getHeader().chunk_ID, 4);
	fout.write((char*)(&thread.getHeader().chunk_size), 4);
	fout.write((char*)thread.getHeader().format, 4);

	fout.write((char*)thread.getHeader().subchunk1_ID, 4);
	fout.write((char*)(&thread.getHeader().subchunk1_size), 4);


	char audio_format[2];
	audio_format[0] = char(thread.getHeader().audio_format & 0xff);
	audio_format[1] = char((thread.getHeader().audio_format >> 8) & 0xff);
	fout.write((char*)audio_format, 2);


	char num_channels[2];
	num_channels[0] = char(thread.getHeader().num_channels & 0xff);
	num_channels[1] = char((thread.getHeader().num_channels >> 8) & 0xff);
	fout.write((char*)num_channels, 2);


	char sample_rate[4];
	sample_rate[0] = char(thread.getHeader().sample_rate & 0xff);
	sample_rate[1] = char((thread.getHeader().sample_rate >> 8) & 0xff);
	sample_rate[2] = char((thread.getHeader().sample_rate >> 16) & 0xff);
	sample_rate[3] = char((thread.getHeader().sample_rate >> 24) & 0xff);
	fout.write((char*)sample_rate, 4);


	char byte_rate[4];
	byte_rate[0] = char(thread.getHeader().byte_rate & 0xff);
	byte_rate[1] = char((thread.getHeader().byte_rate >> 8) & 0xff);
	byte_rate[2] = char((thread.getHeader().byte_rate >> 16) & 0xff);
	byte_rate[3] = char((thread.getHeader().byte_rate >> 24) & 0xff);
	fout.write((char*)byte_rate, 4);


	char block_align[2];
	block_align[0] = char(thread.getHeader().block_align & 0xff);
	block_align[1] = char((thread.getHeader().block_align >> 8) & 0xff);
	fout.write((char*)block_align, 2);


	char bits_per_sample[2];
	bits_per_sample[0] = char(thread.getHeader().bits_per_sample & 0xff);
	bits_per_sample[1] = char((thread.getHeader().bits_per_sample >> 8) & 0xff);
	fout.write((char*)bits_per_sample, 2);

	fout.write((char*)thread.getHeader().subchunk3_ID, 4);
	fout.write((char*)(&thread.getHeader().subchunk3_size), 4);

	fout.write((char*)thread.getData(), thread.getHeader().subchunk3_size);

	fout.close();
	return 0;
}