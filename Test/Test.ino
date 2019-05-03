
 // the setup function runs once when you press reset or power the board
#include <STM32ADC.h>
#include <RTClock.h>



STM32ADC myADC(ADC1);
RTClock rtc(RTCSEL_LSI);

int c = 0;
void rtcInterrupt() {
	Serial.println(rtc.getTime());
}

void setup() {
	Serial.begin(250000);
	//pinMode(PC13, OUTPUT);
	rtc.attachSecondsInterrupt(rtcInterrupt);// Call blink 
}



// the loop function runs over and over again forever
void loop() {
	//digitalWrite(PC13, HIGH);   // turn the LED on (HIGH is the voltage level)
	//delay(100);              // wait for a second
	//digitalWrite(PC13, LOW);    // turn the LED off by making the voltage LOW
	//delay(1000);              // wait for a second
	//Serial.println("FF");

	
}