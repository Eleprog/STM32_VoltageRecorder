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
	PackagePSP pack;
public:
	SetupDateTime(StartBit startBit) {
		this->startBit = startBit;
		pack.init(startBit, du);
	}

	uint32_t getDateTime(int dataByte) {
		/*Serial.print("Byte: ");
		Serial.println(dataByte);*/
		
		if (psp1n.decode(dataByte, pack, matrix)) {			
			return pack.getItem()[0].value;
		}
		return 0;
	}
};

#endif

