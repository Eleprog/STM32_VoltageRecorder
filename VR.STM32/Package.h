// DataMeasure.h

#ifndef _PACKAGE_h
#define _PACKAGE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "Settings.h"
#include "PSP.h"

#define BIT_SIZE_PACKAGE   (BIT_DATETIME + (BIT_ADC*CHANNELS_ADC)) //Размер данных в битах

#if (BIT_SIZE_PACKAGE % 7) > 0
#define PACKAGE_SIZE (BIT_SIZE_PACKAGE / 7) + 1
#else 
#define PACKAGE_SIZE (BIT_SIZE_PACKAGE / 7) 
#endif

class Package
{
protected:	
	PSP psp;
	byte data[PACKAGE_SIZE];
public:
	uint32_t dateTime;
	int millisecond;
	uint16_t measure[CHANNELS_ADC];
	Package()
	{
		psp.init(1, data, PACKAGE_SIZE);
	}
	// указатель на массив байт в пакете
	byte* GetBytes() 
	{
		psp.pushData(32, dateTime);
		psp.pushData(10, millisecond);
		for (byte i = 0; i < CHANNELS_ADC; i++)
		{
			psp.pushData(12, measure[i]);
		}
		return psp.popData();
	}
	//длина пакета
	byte GetLength() 
	{
		return PACKAGE_SIZE;
	}
};

#endif

