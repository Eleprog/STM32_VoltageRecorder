

#include <STM32ADC.h>
#include <RTClock.h>
#include "BuffersController.h"
#include "Package.h"
#include "SdController.h"


STM32ADC myADC(ADC1);
RTClock rtc(RTCSEL_LSI);
BufferController bufferController;
Package package;
Buffer buffer;
int countTimer = 0;

static void DMA1_CH1_Event() {

}


void timerInterrupt() {
	if (countTimer++ >= 999)
	{
		Timer3.pause();
		//Serial.print(countTimer);
		//Serial.println("-------");
		countTimer = 0;
	}
	package.millisecond = countTimer;
	
	//Serial.println("Add package");
	bufferController.AddPackage(package);
	//Serial.write(package.GetBytes(), package.GetLength());
	Serial.println("------");
	for (size_t i = 0; i < 5; i++)
	{
		Serial.println(package.GetBytes()[i], 2);
	}
	Buffer* buff = bufferController.GetReadyBuffer();
	if (buff!=nullptr)
	{
		//TODO: write to SDcard
		sdController.write(buff->GetData());
	}
}

void rtcInterrupt() {
	package.dateTime = rtc.getTime();
	//Serial.println(rtc.getTime());
	Timer3.refresh();
	Timer3.resume();
	//Serial.println(rtc.getTime());	
}

void setup()
{
	//Package p;
	//p.dateTime = 3;
	//Package* n = nullptr;
	////n = &p;

	//while (true)
	//{
	//	delay(5000);
	//	if (n!=nullptr)
	//	{
	//		Serial.println((*n).dateTime);
	//		
	//	}
	//	else {
	//		Serial.println("F");
	//		n = &p;
	//	}
	//	delay(1000);
	//}
	sdController.init();

	Serial.begin(250000);
	package.millisecond = 401;
	package.measure[0] = 202;
	package.measure[1] = 303;
	package.measure[2] = 404;

	Timer3.pause();
	Timer3.setPeriod(1000);
	Timer3.attachCompare1Interrupt(timerInterrupt);
	//Serial.print("1");
	//for (size_t i = 0; i < 10; i++)
	//rtc_init(RTCSEL_LSE);//LSE should be 32768 Hz.
	//rtc_set_prescaler_load(0x7fff);
	{		
		rtc.attachSecondsInterrupt(rtcInterrupt);// Call blink 
	}
	//Serial.print("2");
	myADC.calibrate();
	for (unsigned int j = 0; j < CHANNELS_ADC; j++)
		pinMode(channelsADC[j], INPUT_ANALOG);

	myADC.setSampleRate(ADC_SMPR_1_5);//set the Sample Rate
	myADC.setScanMode();              //set the ADC in Scan mode. 
	myADC.setPins(channelsADC, CHANNELS_ADC);           //set how many and which pins to convert.
	myADC.setContinuous();            //set the ADC in continuous mode.

  //set the DMA transfer for the ADC. 
  //in this case we want to increment the memory side and run it in circular mode
  //By doing this, we can read the last value sampled from the channels by reading the dataPoints array
	myADC.setDMA(package.measure, CHANNELS_ADC, (DMA_MINC_MODE | DMA_CIRC_MODE), NULL);
	myADC.startConversion();
}


void loop()
{

}