

#include "SetupDateTime.h"
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
SetupDateTime setupDateTime(StartBitPSP1N::ZERO);


bool bufferReady = false;

int countTimer = 0;
void timerInterrupt() {
	if (countTimer++ >= TIMER_RESET_COUNTER)
	{
		Timer3.pause();
		package.dateTime = rtc.getTime();
		//Serial.println(millis());
		//Serial.println(package.dateTime);
		countTimer = 0;		
	}
	package.millisecond = countTimer * TIME_FACTOR;
	byte* packEncode = package.Encode().getData();
	bufferReady = ringBuffer.AddData(packEncode);

	
	Serial.write(packEncode, PACKAGE_SIZE);
	
}

int c = 0;
void rtcInterrupt() {
	Timer3.refresh();
	Timer3.resume();
}

void setup() {
	Serial.begin(250000, SERIAL_8O1);
	pinMode(PIN_CONTROL_POWER, INPUT);
	Timer3.pause();
	Timer3.setPeriod(SAMPLING_TIME);
	Timer3.attachCompare1Interrupt(timerInterrupt);

	rtc.attachSecondsInterrupt(rtcInterrupt);// Call blink 
	//sdController.init();
	 
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
	/*if (bufferReady)
	{
		while (ringBuffer.Count() > 0 && digitalRead(PIN_CONTROL_POWER) == HIGH)
		{
			byte* buf = ringBuffer.GetData();
			String fileName;
			fileName += rtc.day();
			fileName += "_";
			fileName += rtc.month();
			fileName += "_";
			fileName += rtc.year() + 1970;
			fileName += ".bvr";
			sdController.write(buf, BUFFER_SIZE, fileName);
		}
		bufferReady = false;
	}*/
	if (Serial.available() > 0)
	{
		uint32_t time = 0;
		rtc.detachSecondsInterrupt();
		Timer3.detachCompare1Interrupt();
		for (size_t i = 0; i < 5; i++)
		{
			time = setupDateTime.getDateTime(Serial.read());
		}
		if (time > 0)
		{
			rtc.setTime(time);

		}

		rtc.attachSecondsInterrupt(rtcInterrupt);// Call blink 
		Timer3.attachCompare1Interrupt(timerInterrupt);
	}
}
//convert string to uint32_t
uint32_t strToUl(String string) {
	uint32_t len = string.length();
	uint32_t result = 0;

	for (size_t i = 0; i < len; i++)
	{
		if ((byte)string[i] >= '0' && (byte)string[i] <= '9') {
			result = result * 10 + string[i] - '0';

		}
		else { break; }
	}
	return result;
}
