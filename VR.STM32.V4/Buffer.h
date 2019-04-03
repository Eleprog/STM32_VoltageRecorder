// Buffer.h

#ifndef _BUFFER_h
#define _BUFFER_h
#include "Package.h"

class Buffer
{
protected:
	byte arrayByte[BUFFER_SIZE];
	uint16_t position = 0;
public:
	bool AddData(byte* packageEncode) {
		for (size_t i = 0; i < PACKAGE_SIZE; i++)
		{
			arrayByte[position + i] = packageEncode[i];
		}
		position += PACKAGE_SIZE;
		if (position >= BUFFER_SIZE) {
			position = 0;
			return true;
		}
		return false;
	}

	byte* GetData() {
		return arrayByte;
	}
};


#endif

