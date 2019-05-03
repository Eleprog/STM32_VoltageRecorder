// SdController.h

#ifndef _SDCONTROLLER_h
#define _SDCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <SysCall.h>
#include <sdios.h>
#include <SdFatConfig.h>
#include <SdFat.h>
#include <MinimumSerial.h>
#include <FreeStack.h>
#include <BlockDriver.h>
#include "Settings.h"
#include <SPI.h>


// Use second SPI port
SPIClass SPI_2(2);
SdFat sd2(&SPI_2);
const uint8_t SD2_CS = PB12;

class SdControllerClass
{
 protected:
	 SdFat sd;
	 //File file;

 public:
	 SdControllerClass() {
		 pinMode(PIN_ERR_SD, OUTPUT);
	 }
	 void init() {
		 
		 while (!sd2.begin(SD2_CS, SD_SCK_MHZ(18)))
		 {		
			
			 Serial.println("SD card not init");
			 digitalWrite(PIN_ERR_SD, 1);
			 delay(300);
			 digitalWrite(PIN_ERR_SD, 0);
			 delay(300);
			 //nvic_sys_reset();
			 //return false;
		 }
		// return true;
	 }
	 void write(byte* buffer, uint16_t size, String& fileName) {
		 String _fileName = fileName;
		 //_fileName += ".bvr";	
		File file = sd2.open(_fileName, FILE_WRITE);
		 
		if (file)
		 {			 
			 file.write(buffer, size);
			 //USART0.send(file);
			 file.close();
		 }
		 else
		 {
			Serial.println("error opening file");
			//nvic_sys_reset();			
			 init();
		 }
	 }

}sdController;

#endif

