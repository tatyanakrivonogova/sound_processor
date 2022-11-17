#include "Thread.h"
#include "WAVheader.h"
#include <memory>

Thread::Thread(const Thread& otherThread) : header(otherThread.header), data(otherThread.data) {}

Thread::Thread(Thread&& otherThread) noexcept {
    header = std::move(otherThread.header);
    data = std::move(otherThread.data);
}

Thread& Thread::operator=(const Thread& otherThread) {
    if (this == &otherThread)
        return *this;

    header = WAVheader(otherThread.header);
    data = otherThread.data;
    return *this;
}

Thread& Thread::operator=(Thread&& otherThread) noexcept {
    if (this == &otherThread)
        return *this;
    header = std::move(otherThread.header);
    data = std::move(otherThread.data);
    return *this;
}

WAVheader& Thread::getHeader() {
	return header;
}

std::vector<char>& Thread::getData() {
	return data;
}

void Thread::setData(std::vector<char>& newData) {

    data.assign(newData.begin(), newData.end());
}