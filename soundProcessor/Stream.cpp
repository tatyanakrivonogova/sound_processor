#include "Stream.h"
#include "WAVheader.h"
#include <memory>

Stream::Stream(const Stream& otherStream) : header(otherStream.header), data(otherStream.data), number_of_samples(otherStream.number_of_samples), file(otherStream.file){}

Stream::Stream(Stream&& otherStream) noexcept {
    header = std::move(otherStream.header);
    data = std::move(otherStream.data);
    number_of_samples = std::move(number_of_samples);
    file = otherStream.file;
}

Stream& Stream::operator=(const Stream& otherStream) {
    if (this == &otherStream)
        return *this;

    header = WAVheader(otherStream.header);
    data = otherStream.data;
    number_of_samples = otherStream.number_of_samples;
    file = otherStream.file;
    return *this;
}

Stream& Stream::operator=(Stream&& otherStream) noexcept {
    if (this == &otherStream)
        return *this;
    header = std::move(otherStream.header);
    data = std::move(otherStream.data);
    number_of_samples = std::move(number_of_samples);
    file = otherStream.file;
    return *this;
}

WAVheader& Stream::getHeader() {
	return header;
}

const unsigned int Stream::getData() {
	return data;
}

const unsigned int Stream::getNumberOfSamples() {
    return number_of_samples;
}

std::shared_ptr<std::string> Stream::getFile() {
    return file;
}

void Stream::setHeader(const WAVheader& newHeader) {
    header = newHeader;
}

void Stream::setData(unsigned int newData) {
    data = newData;
}

void Stream::setNumberOfSamples(unsigned int new_number_of_samples) {
    number_of_samples = new_number_of_samples;
}

void Stream::setFile(std::shared_ptr<std::string> newSource) {
    file = newSource;
}