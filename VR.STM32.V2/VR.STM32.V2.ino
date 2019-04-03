
#include <SPI.h>
#include <SysCall.h>
#include <sdios.h>
#include <SdFatConfig.h>
#include <SdFat.h>
#include <MinimumSerial.h>
#include <FreeStack.h>
#include <BlockDriver.h>
#include "SDController.h"
#include "BufferController.h"
#include "Buffer.h"
#include <STM32ADC.h>
#include <RTClock.h>
#include "Package.h"
#include "Settings.h"
#include "SDController.h"


STM32ADC myADC(ADC1);
RTClock rtc(RTCSEL_LSE);
//PackagePSP package;
BufferController bufferController;
/* END Calculation of the size of the matrix byte*/
Package package;

int countTimer = 0;
bool isBufferReady = false;

void timerInterrupt() {
	if (countTimer++ >= 499)
	{
		Timer3.pause();
		package.dateTime = rtc.getTime();
		countTimer = 0;
	}
	package.milliseconds = countTimer * 2;
	//package.measure[5] = 0b111111111111;
	/*Serial.println(package.measure[5]);
	for (size_t i = 0; i < 24; i++)
	{
		Serial.println(package.encode()[i],2);
	}
	Serial.println("----");*/
	/*Serial.println(package.dateTime);
	Serial.println(package.milliseconds);
	Serial.println("-----");*/
	/*psp.encode(packagePSPEncode, matrixEncode);
	for (size_t i = 0; i < MATRIX_SIZE; i++)
	{
		Serial.println(matrixEncode[i], 2);
	}*/
	isBufferReady = bufferController.addPackage(package);
	//Serial.println(pacl);
	byte* b = package.encode();
	Serial.write(b, package.sizeByte);
	

	//for (size_t i = 0; i < package.sizeByte; i++)
	//{
	//	Serial.println(b[i],2);
	//}
	//Serial.println("----");

}

void rtcInterrupt() {
	Timer3.refresh();
	Timer3.resume();
}

void setup() {
	Timer3.pause();
	Timer3.setPeriod(2000);
	Timer3.attachCompare1Interrupt(timerInterrupt);

	rtc.attachSecondsInterrupt(rtcInterrupt);

	myADC.calibrate();
	for (unsigned int j = 0; j < CHANNELS_ADC; j++)
		pinMode(channelsADC[j], INPUT_ANALOG);

	myADC.setSampleRate(ADC_SMPR_1_5);//set the Sample Rate
	myADC.setScanMode();              //set the ADC in Scan mode. 
	myADC.setPins(channelsADC, CHANNELS_ADC);           //set how many and which pins to convert.
	myADC.setContinuous();
	myADC.setDMA(package.measure, NUMBER_OF_CHANNELS_ADC, (DMA_MINC_MODE | DMA_CIRC_MODE), NULL);
	myADC.startConversion();
}


void loop() {
	if (isBufferReady)
	{
		while (bufferController.getCount() > 0)
		{
			sdController.write(bufferController.popBuffer());
		}

		bufferController.clearFlagBufferReady();
	}
}
