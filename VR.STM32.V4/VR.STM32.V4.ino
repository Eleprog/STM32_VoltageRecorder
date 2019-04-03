

#include <STM32ADC.h>
#include <RTClock.h>
#include "Package.h"
#include "Settings.h"
#include "RingBuffer.h"
#include "SdController.h"

STM32ADC myADC(ADC1);
RTClock rtc(RTCSEL_LSE);
Package package(ZERO);
RingBuffer ringBuffer;

bool bufferReady = false;

int countTimer = 0;
void timerInterrupt() {
	if (countTimer++ >= TIMER_RESET_COUNTER)
	{
		Timer3.pause();
		package.dateTime = rtc.getTime();
		countTimer = 0;
	}
	package.millisecond = countTimer * TIME_FACTOR;
	byte* packEncode = package.Encode();
	bufferReady = ringBuffer.AddData(packEncode);

	Serial.write(packEncode, PACKAGE_SIZE);	
}

void rtcInterrupt() {
	
	Timer3.refresh();
	Timer3.resume();
}

void setup() {
	Serial.begin(250000);
	Timer3.pause();
	Timer3.setPeriod(SAMPLING_TIME);
	Timer3.attachCompare1Interrupt(timerInterrupt);

	rtc.attachSecondsInterrupt(rtcInterrupt);// Call blink 
	rtc.setTime(1554269650);
	myADC.calibrate();
	for (unsigned int j = 0; j < CHANNELS_ADC; j++)
		pinMode(channelsADC[j], INPUT_ANALOG);

	myADC.setSampleRate(ADC_SMPR_1_5);//set the Sample Rate
	myADC.setScanMode();              //set the ADC in Scan mode. 
	myADC.setPins(channelsADC, CHANNELS_ADC);           //set how many and which pins to convert.
	myADC.setContinuous();
	myADC.setDMA(package.measure, CHANNELS_ADC, (DMA_MINC_MODE | DMA_CIRC_MODE), NULL);
	myADC.startConversion();
}

void loop() {
	if (bufferReady)
	{
		while (ringBuffer.Count() > 0)
		{
			byte* buf = ringBuffer.GetData();
		/*	for (size_t i = 0; i < PACKETS_IN_BUFFER; i += PACKAGE_SIZE)
			{
				Serial.println(buf[i], 2);
				Serial.println(buf[i + 1], 2);
				Serial.println(buf[i + 2], 2);
				Serial.println(buf[i + 3], 2);
				Serial.println(buf[i + 23], 2);
			}
			Serial.println("-----");*/
			sdController.write(buf,BUFFER_SIZE);
		}
		bufferReady = false;
	}
}
