// Buffer.h

#ifndef _BUFFER_h
#define _BUFFER_h


#include "Package.h"
#include "Settings.h"

#define PACKAGE_IN_BUFFER_MAX (BUFFER_SIZE / PACKAGE_SIZE_BYTE)

class Buffer
{
protected:
	byte buffer[BUFFER_SIZE];
	int position = 0;
	byte countPackage = 0;
	bool isFull = false;
public:
	bool addPackage(Package& package) {
		if (!isFull)
		{
			byte* b = package.encode();
			for (byte i = 0; i < package.sizeByte; i++)
			{
				buffer[position + i] = b[i];
			}
			position += package.sizeByte;
			countPackage++;
			if (countPackage >= PACKAGE_IN_BUFFER_MAX)
			{
				isFull = true;
			}
		}
		return isFull;
	}
	byte* getData() {
		if (isFull)
		{
			position = 0;
			countPackage = 0;
			isFull = false;
			return buffer;
		}
		return nullptr;
	}
};


#endif

