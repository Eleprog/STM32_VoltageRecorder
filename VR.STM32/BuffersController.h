

#ifndef _BUFFERSCONTROLLER_h
#define _BUFFERSCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "Buffer.h"
#include "Settings.h"
#include "Package.h"

class BufferController
{
protected:
	Buffer buffers[NUMBER_OF_BUFFER];
	byte position = 0;
	Buffer* currentBuffer = nullptr;
	void nextBuffer() {
		if (position < NUMBER_OF_BUFFER-1)
		{
			position++;
		}
		else {
			position = 0;
		}
	}
	bool bufferReady = false;
public:
	int GetSizeBuffer() {
		return SD_BUFFER_SIZE;
	}

	void AddPackage(Package &package) {
		if (buffers[position].AddPackage(package))
		{
			currentBuffer = &buffers[position];
			bufferReady = true;
			buffers[position].ClearFlagFull();
			//Serial.println(position);
			//Serial.println(buffers[position].IsFull());
			nextBuffer();			
		}
	}

	bool IsBufferReady() {
		return bufferReady;
	}

	Buffer* GetReadyBuffer() {
		if (bufferReady)
		{
			bufferReady = false;
			return currentBuffer;
		}
		return nullptr;
	}
};


#endif

