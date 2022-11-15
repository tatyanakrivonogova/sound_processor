#include "Thread.h"
#include "WAVheader.h"

WAVheader& Thread::getHeader() {
	return header;
}

char* Thread::getData() {
	return data;
}

void Thread::setData(char* newData) {
	if (data != nullptr) {
		delete[] data;
	}
	data = newData;
}