#include "outputThread.h"
#include "WAVheader.h"
#include "Thread.h"
#include "outputHeader.h"
#include <iostream>
#include <iosfwd>
#define BUF_SIZE 1000

int outputThread::output() {
	FILE* fout;
	fopen_s(&fout, "new_Ring08.wav", "wb");
	if (!fout) {
		throw std::runtime_error("Unavailable output file");
	}

	outputHeader outputHeader(fout, thread.getHeader());
	outputHeader.output();


	FILE* fin;
	fopen_s(&fin, (*thread.getFile()).c_str(), "rb");
	if (!fin) {
		throw std::runtime_error("Unavailable input file");
	}

	fseek(fin, thread.getData(), SEEK_SET);
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