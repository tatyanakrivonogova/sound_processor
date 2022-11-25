#include "Thread.h"
#include "WAVheader.h"
#include <memory>

Thread::Thread(const Thread& otherThread) : header(otherThread.header), data(otherThread.data), number_of_samples(number_of_samples), file(otherThread.file){}

Thread::Thread(Thread&& otherThread) noexcept {
    header = std::move(otherThread.header);
    data = std::move(otherThread.data);
    number_of_samples = std::move(number_of_samples);
    file = otherThread.file;
}

Thread& Thread::operator=(const Thread& otherThread) {
    if (this == &otherThread)
        return *this;

    header = WAVheader(otherThread.header);
    data = otherThread.data;
    number_of_samples = otherThread.number_of_samples;
    file = otherThread.file;
    return *this;
}

Thread& Thread::operator=(Thread&& otherThread) noexcept {
    if (this == &otherThread)
        return *this;
    header = std::move(otherThread.header);
    data = std::move(otherThread.data);
    number_of_samples = std::move(number_of_samples);
    file = otherThread.file;
    return *this;
}

WAVheader& Thread::getHeader() {
	return header;
}

const unsigned int Thread::getData() {
	return data;
}

const unsigned int Thread::getNumberOfSamples() {
    return number_of_samples;
}

std::shared_ptr<std::string> Thread::getFile() {
    return file;
}

void Thread::setHeader(const WAVheader& newHeader) {
    header = newHeader;
}

void Thread::setData(unsigned int newData) {
    data = newData;
}

void Thread::setNumberOfSamples(unsigned int new_number_of_samples) {
    number_of_samples = new_number_of_samples;
}

void Thread::setFile(std::shared_ptr<std::string> newSource) {
    file = newSource;
}