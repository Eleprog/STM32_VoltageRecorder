// SetupDateTime.h

#ifndef _SETUPDATETIME_h
#define _SETUPDATETIME_h

#include "Settings.h"
#include "PSP1N_V2.h"

#define DATETIME_BYTES 5

class SetupDateTime
{
protected:
	StructurePackagePSP1N structurePack[1] = { 32 };
	byte bufferPack[5];
	PackagePSP1N pack;
public:
	SetupDateTime(StartBitPSP1N startBit) {
		pack.init(startBit, structurePack, bufferPack);
	}

	byte* encode() {
		return pack.encode().getData();
	}

	uint32_t getDateTime(int dataByte) {
		if (pack.decode(dataByte)==ResultDecodePSP1N::DECODE_OK)
		{
			return pack.getItemValue(0);
		}
		return 0;
	}
};

#endif

