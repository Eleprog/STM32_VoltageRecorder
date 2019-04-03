// Package.h

#ifndef _PACKAGE_h
#define _PACKAGE_h
#include "Settings.h"
#include "PSP1N.h"

class Package
{
 protected:	 
	 PSP psp;
	 byte arrayData[PACKAGE_SIZE];
 public:	 
	 uint32_t dateTime = 0;
	 uint16_t millisecond = 0;
	 uint16_t measure[CHANNELS_ADC];
	 Package(StartBit startBit) {
		 psp.init(startBit, arrayData);
	 }
	 byte* Encode() {
		 psp.push(SIZE_BIT_DATE_TIME, dateTime);
		 psp.push(SIZE_BIT_MILLIS, millisecond);
		 for (size_t i = 0; i < CHANNELS_ADC; i++)
		 {
			 psp.push(SIZE_BIT_ADC, measure[i]);
		 }		 
		 return psp.getData();
	 }
};

#endif

