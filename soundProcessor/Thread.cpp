#include "Thread.h"
#include "WAVheader.h"
#include <memory>

Thread::Thread(const Thread& otherThread) : header(otherThread.header), data(otherThread.data), file(otherThread.file){}

Thread::Thread(Thread&& otherThread) noexcept {
    header = std::move(otherThread.header);
    data = std::move(otherThread.data);
    file = otherThread.file;
}

Thread& Thread::operator=(const Thread& otherThread) {
    if (this == &otherThread)
        return *this;

    header = WAVheader(otherThread.header);
    data = otherThread.data;
    file = otherThread.file;
    return *this;
}

Thread& Thread::operator=(Thread&& otherThread) noexcept {
    if (this == &otherThread)
        return *this;
    header = std::move(otherThread.header);
    data = std::move(otherThread.data);
    file = otherThread.file;
    return *this;
}

WAVheader& Thread::getHeader() {
	return header;
}

unsigned int Thread::getData() {
	return data;
}

std::shared_ptr<std::string> Thread::getFile() {
    return file;
}

void Thread::setData(unsigned int newData) {
    data = newData;
}

void Thread::setHeader(WAVheader& newHeader) {
    header = newHeader;
}

void Thread::setFile(std::shared_ptr<std::string> newSource) {
    file = newSource;
}