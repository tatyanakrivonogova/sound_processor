#include "Thread.h"
#include "WAVheader.h"
#include <memory>

Thread::Thread(const Thread& otherThread) : header(otherThread.header), data(otherThread.data), source(otherThread.source){}

Thread::Thread(Thread&& otherThread) noexcept {
    header = std::move(otherThread.header);
    data = std::move(otherThread.data);
    source = otherThread.source;
}

Thread& Thread::operator=(const Thread& otherThread) {
    if (this == &otherThread)
        return *this;

    header = WAVheader(otherThread.header);
    data = otherThread.data;
    source = otherThread.source;
    return *this;
}

Thread& Thread::operator=(Thread&& otherThread) noexcept {
    if (this == &otherThread)
        return *this;
    header = std::move(otherThread.header);
    data = std::move(otherThread.data);
    source = otherThread.source;
    return *this;
}

WAVheader& Thread::getHeader() {
	return header;
}

unsigned int Thread::getData() {
	return data;
}

std::shared_ptr<std::string> Thread::getSource() {
    return source;
}

void Thread::setData(unsigned int newData) {
    data = newData;
}

void Thread::setHeader(WAVheader& newHeader) {
    header = newHeader;
}

void Thread::setSource(std::shared_ptr<std::string> newSource) {
    source = newSource;
}