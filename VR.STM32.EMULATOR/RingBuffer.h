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
		//if (writeCount >= NUMBER_OF_BUFFERS) writeCount = 0;
	}
	void read() {
		readCount++;
		readCount &= mask;
		//if (readCount >= NUMBER_OF_BUFFERS) readCount = 0;
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
			buf = buffers[readCount].GetData();
			
			read();			
		}
	/*	for (size_t i = 0; i < PACKETS_IN_BUFFER; i += PACKAGE_SIZE)
		{
			Serial.println(buf[i], 2);
			Serial.println(buf[i + 1], 2);
			Serial.println(buf[i + 2], 2);
			Serial.println(buf[i + 3], 2);
			Serial.println(buf[i + 23], 2);
		}
		Serial.println("-----");*/
		return buf;

	}
	uint16_t Count() {
		return (writeCount - readCount) & mask;
		/*byte count = 0;
		if (writeCount < readCount)
		{
			count = NUMBER_OF_BUFFERS + writeCount - readCount;
		}
		else {
			count = writeCount - readCount;
		}
		return count;*/
	}
};


#endif

