#include "WAVheader.h"
#include <memory>

unsigned int& WAVheader::get_chunk_size() {
    return chunk_size;
}
unsigned int& WAVheader::get_subchunk1_size() {
    return subchunk1_size;
}
unsigned int& WAVheader::get_audio_format() {
    return audio_format;
}
unsigned int& WAVheader::get_num_channels() {
    return num_channels;
}
unsigned int& WAVheader::get_sample_rate() {
    return sample_rate;
}
unsigned int& WAVheader::get_byte_rate() {
    return byte_rate;
}
unsigned int& WAVheader::get_block_align() {
    return block_align;
}
unsigned int& WAVheader::get_bits_per_sample() {
    return bits_per_sample;
}
unsigned int& WAVheader::get_subchunk3_size() {
    return subchunk3_size;
}

char* WAVheader::get_chunk_ID() {
    return chunk_ID;
}
char* WAVheader::get_format() {
    return format;
}
char* WAVheader::get_subchunk1_ID() {
    return subchunk1_ID;
}
char* WAVheader::get_subchunk3_ID() {
    return subchunk3_ID;
}

WAVheader::WAVheader(const WAVheader& otherStream) : chunk_size(otherStream.chunk_size), subchunk1_size(otherStream.subchunk1_size),
audio_format(otherStream.audio_format), num_channels(otherStream.num_channels), sample_rate(otherStream.sample_rate),
byte_rate(otherStream.byte_rate), block_align(otherStream.block_align), bits_per_sample(otherStream.bits_per_sample),
subchunk3_size(otherStream.subchunk3_size) {
    for (int i = 0; i < 4; ++i) {
        chunk_ID[i] = otherStream.chunk_ID[i];
    }

    for (int i = 0; i < 4; ++i) {
        format[i] = otherStream.format[i];
    }

    for (int i = 0; i < 4; ++i) {
        subchunk1_ID[i] = otherStream.subchunk1_ID[i];
    }

    for (int i = 0; i < 4; ++i) {
        subchunk3_ID[i] = otherStream.subchunk3_ID[i];
    }
}

WAVheader::WAVheader(WAVheader&& otherStream) noexcept : chunk_size(otherStream.chunk_size), subchunk1_size(otherStream.subchunk1_size),
audio_format(otherStream.audio_format), num_channels(otherStream.num_channels), sample_rate(otherStream.sample_rate),
byte_rate(otherStream.byte_rate), block_align(otherStream.block_align), bits_per_sample(otherStream.bits_per_sample),
subchunk3_size(otherStream.subchunk3_size) {
    for (int i = 0; i < 4; ++i) {
        chunk_ID[i] = otherStream.chunk_ID[i];
    }

    for (int i = 0; i < 4; ++i) {
        format[i] = otherStream.format[i];
    }

    for (int i = 0; i < 4; ++i) {
        subchunk1_ID[i] = otherStream.subchunk1_ID[i];
    }

    for (int i = 0; i < 4; ++i) {
        subchunk3_ID[i] = otherStream.subchunk3_ID[i];
    }
}

WAVheader& WAVheader::operator=(const WAVheader& otherStream) {
    if (this == &otherStream)
        return *this;

    chunk_size = otherStream.chunk_size;
    subchunk1_size = otherStream.subchunk1_size;
    audio_format = otherStream.audio_format;
    num_channels = otherStream.num_channels;
    sample_rate = otherStream.sample_rate;
    byte_rate = otherStream.byte_rate;
    block_align = otherStream.block_align;
    bits_per_sample = otherStream.bits_per_sample;
    subchunk3_size = otherStream.subchunk3_size;

    for (int i = 0; i < 4; ++i) {
        chunk_ID[i] = otherStream.chunk_ID[i];
    }

    for (int i = 0; i < 4; ++i) {
        format[i] = otherStream.format[i];
    }

    for (int i = 0; i < 4; ++i) {
        subchunk1_ID[i] = otherStream.subchunk1_ID[i];
    }

    for (int i = 0; i < 4; ++i) {
        subchunk3_ID[i] = otherStream.subchunk3_ID[i];
    }

    return *this;
}

WAVheader& WAVheader::operator=(WAVheader&& otherStream) noexcept {
    if (this == &otherStream)
        return *this;

    chunk_size = otherStream.chunk_size;
    subchunk1_size = otherStream.subchunk1_size;
    audio_format = otherStream.audio_format;
    num_channels = otherStream.num_channels;
    sample_rate = otherStream.sample_rate;
    byte_rate = otherStream.byte_rate;
    block_align = otherStream.block_align;
    bits_per_sample = otherStream.bits_per_sample;
    subchunk3_size = otherStream.subchunk3_size;

    for (int i = 0; i < 4; ++i) {
        chunk_ID[i] = otherStream.chunk_ID[i];
    }

    for (int i = 0; i < 4; ++i) {
        format[i] = otherStream.format[i];
    }

    for (int i = 0; i < 4; ++i) {
        subchunk1_ID[i] = otherStream.subchunk1_ID[i];
    }

    for (int i = 0; i < 4; ++i) {
        subchunk3_ID[i] = otherStream.subchunk3_ID[i];
    }

    return *this;
}