// Package.h

#ifndef _PACKAGE_h
#define _PACKAGE_h

#include "PSP1N.h"


#define NUMBER_OF_CHANNELS_ADC 10  


class Package
{
 protected:
	 byte arrayByte[PACKAGE_SIZE_BYTE];
	 PSP psp;
	 

 public:
	 StartBit startBit = ONE;
	 uint32_t dateTime = 0;
	 uint16 milliseconds = 0;
	 uint16 measure[NUMBER_OF_CHANNELS_ADC];
	 const byte sizeByte = PACKAGE_SIZE_BYTE;

	 Package() {
		 psp.init(startBit, arrayByte);
	 }

	 byte* encode() {
		 psp.push(32, dateTime);
		 psp.push(10, milliseconds);
		 for (byte i = 0; i < NUMBER_OF_CHANNELS_ADC; i++)
		 {
			 psp.push(12, measure[i]);
		 }
		 return psp.getData();
	 }

};

#endif

