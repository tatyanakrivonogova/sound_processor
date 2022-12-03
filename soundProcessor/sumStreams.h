#pragma once
#include <climits>
#include "readBuffer.h"
#include "writeBuffer.h"


inline void sumStreams(readBuffer& readBuff1, readBuffer& readBuff2, writeBuffer& writeBuff, size_t size1, size_t size2,
	size_t begin1, size_t begin2, size_t end, double ratio1 = 1.0, double ratio2 = 1.0) {
	for (size_t i1 = begin1, i2 = begin2; i1 < end; ++i1, ++i2) {
		int sample = 0;
		if (size1 > i1) {
			sample += static_cast<int>(ratio1 * readBuff1[i1]);
		}
		if (size2 > i2) {
			sample += static_cast<int>(ratio2 * readBuff2[i2]);
		}
		if (sample > SHRT_MAX) {
			writeBuff >> SHRT_MAX;
		}
		else if (sample < SHRT_MIN) {
			writeBuff >> SHRT_MIN;
		}
		else {
			writeBuff >> static_cast<short>(sample);
		}
	}
}