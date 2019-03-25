// Buffer.h

#ifndef _BUFFER_h
#define _BUFFER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "Settings.h"
#include "Package.h"

class Buffer
{
protected:
	void copyArray(byte * dest, int pos, const byte *source, int size) {
		for (int i = 0; i < size; i++)
		{
			dest[pos + i] = source[i];
		}
	}
	byte data[SD_BUFFER_SIZE]; //данные
	int position = 0;
	bool full = false;
public:
	byte* GetData() {
		return data;
	}
	int GetSize() {
		return SD_BUFFER_SIZE;
	}
	bool AddPackage(Package &package) {
		if (!full)
		{
			copyArray(data, position, package.GetBytes(), package.GetLength());
			position += package.GetLength();
			//Serial.println(position);
			if (position > SD_BUFFER_SIZE - package.GetLength())
			{
				position = 0;
				full = true;				
			}
		}
		return full;
	}
	bool IsFull() {
		return full;
	}
	void ClearFlagFull() {
		full = false;
	}
};


#endif

