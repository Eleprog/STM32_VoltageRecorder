// Package.h

#ifndef _PACKAGE_h
#define _PACKAGE_h
#include "Settings.h"
#include "PSP1N_V2.h"

StructurePackagePSP1N structurePack[] = {
SIZE_BIT_DATE_TIME,
SIZE_BIT_MILLIS,
SIZE_BIT_ADC,
SIZE_BIT_ADC,
SIZE_BIT_ADC,
SIZE_BIT_ADC,
SIZE_BIT_ADC,
SIZE_BIT_ADC,
SIZE_BIT_ADC,
SIZE_BIT_ADC,
SIZE_BIT_ADC,
SIZE_BIT_ADC
};

class Package
{
protected:
	byte bufferPack[PACKAGE_SIZE];
	PackagePSP1N pack;
public:
	uint32_t dateTime = 0;
	uint16_t millisecond = 0;
	uint16_t measure[CHANNELS_ADC];
	Package(StartBitPSP1N startBit) {
		pack.init(startBit, structurePack, bufferPack);
	}
	DataPSP1N Encode() {
		pack.setItemValue(0, dateTime);
		pack.setItemValue(1, millisecond);
		for (size_t i = 0; i < PACKAGE_SIZE; i++)
		{
			pack.setItemValue(i + 2, measure[i]);
		}
		return pack.encode();
	}
};

#endif

