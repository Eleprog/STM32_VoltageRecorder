// CyclicBuffer.h

#ifndef _CYCLICBUFFER_h
#define _CYCLICBUFFER_h
#include "Buffer.h"
#include "Settings.h"

class RingBuffer
{
protected:
	Buffer buffers[NUMBER_OF_BUFFERS];
	const byte mask = NUMBER_OF_BUFFERS - 1;
	byte writeCount = 0;
	byte readCount = 0;
	void write() {
		writeCount++;
		writeCount &= mask;
	}
	void read() {
		readCount++;
		readCount &= mask;
	}
public:
	bool AddData(byte* packageEncode) {
		if (buffers[writeCount].AddData(packageEncode))
		{
			write();
			return true;
		}
		return false;
	}
	byte* GetData() {
		byte* buf = nullptr;
		if (readCount != writeCount) {
			byte* buf = buffers[readCount].GetData();
			read();
		}
		return buf;
	}
	uint16_t Count() {
		return (writeCount - readCount) & mask;
	}
};


#endif

