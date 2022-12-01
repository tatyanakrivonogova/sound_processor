#include "outputStream.h"
#include "WAVheader.h"
#include "Stream.h"
#include "outputHeader.h"
#include <iostream>
#include <iosfwd>
#define BUF_SIZE 1000

int outputStream::output() {
	FILE* fout;
	fopen_s(&fout, "new_Ring08.wav", "wb");
	if (!fout) {
		throw std::runtime_error("Unavailable output file");
	}

	outputHeader outputHeader(fout, stream.getHeader());
	outputHeader.output();


	FILE* fin;
	fopen_s(&fin, (*stream.getFile()).c_str(), "rb");
	if (!fin) {
		throw std::runtime_error("Unavailable input file");
	}

	fseek(fin, stream.getData(), SEEK_SET);
	short* buffer = new short[BUF_SIZE];

	for (unsigned int i = 0; i < stream.getHeader().get_subchunk3_size() / BUF_SIZE; ++i) {

		fread(buffer, 2, BUF_SIZE, fin);
		fwrite(buffer, 2, BUF_SIZE, fout);
	}

	fread(buffer, 2, stream.getHeader().get_subchunk3_size() % BUF_SIZE, fin);
	fwrite(buffer, 2, stream.getHeader().get_subchunk3_size() % BUF_SIZE, fout);

	delete[] buffer;
	fclose(fout);
	fclose(fin);
	return 0;
}