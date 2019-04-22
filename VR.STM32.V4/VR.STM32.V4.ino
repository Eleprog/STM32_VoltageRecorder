

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

#define ttd "@ttd%g11#"
#define tim 12552

int countTimer = 0;
void timerInterrupt() {
	if (countTimer++ >= TIMER_RESET_COUNTER)
	{
		Timer3.pause();
		package.dateTime = rtc.getTime();
		countTimer = 0;
	}
	package.millisecond = countTimer * TIME_FACTOR;
	byte* packEncode = package.Encode().getData();
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
	}
	if (Serial.available() > 0)
	{
		uint32_t time = 0;
		rtc.detachSecondsInterrupt();
		Timer3.detachCompare1Interrupt();
		for (size_t i = 0; i < 5; i++)
		{
			time = setupDateTime.getDateTime(Serial.read());
		}
		if (time>0)
		{
			rtc.setTime(time);
			//delay(100);
			//Serial.println("FF");
			//Serial.println(rtc.getTime());
		}
		////Timer3.pause();
		////Serial.println("Available");
		//for (size_t i = 0; i < 5; i++)
		//{
		//	uint32_t synchroneTime = setupDateTime.getDateTime(Serial.read());
		//	uint32_t currTime = rtc.getTime();
		//	uint32_t currTime2;
		//	uint32_t maxTime = 1571356800;
		//	if (currTime>=300)
		//	{
		//		currTime2 = currTime - 300;
		//	}
		//	else currTime2 = currTime;
		//	//Serial.print("currTime2 ");
		//	//Serial.println(currTime2);
		//	if (synchroneTime > currTime2  && synchroneTime <= maxTime)
		//	//if (synchroneTime > 0)
		//	{	
		//		rtc.setTime(synchroneTime);
		//		Serial.print("dTime: ");
		//		Serial.println(rtc.getTime());				
		//		break;
		//	}
		//}	
		rtc.attachSecondsInterrupt(rtcInterrupt);// Call blink 
		Timer3.attachCompare1Interrupt(timerInterrupt);
	}
	//Serial.println(rtc.getTime());
	//delay(1000);
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
