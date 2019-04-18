// SetupDateTime.h

#ifndef _SETUPDATETIME_h
#define _SETUPDATETIME_h

#include "PSP1N.h"

#define DATETIME_BYTES 5

class SetupDateTime
{
 protected:
	 StartBit startBit;
	 byte matrix[DATETIME_BYTES];
	 DataUnit du[1] = { (32) };
	 PSP1N psp1n;

 public:
	 SetupDateTime(StartBit startBit) {
		 this->startBit = startBit;
	 }

	 uint32_t getDateTime() {
		 int _byte;
		 PackagePSP pack(startBit, du);
		 for (size_t i = 0; i < DATETIME_BYTES; i++)
		 {
			 _byte = Serial.read();
			 psp1n.decode(_byte, pack, matrix);
		 }
		 return pack.getItem()[0].value;
	 }
};

#endif

