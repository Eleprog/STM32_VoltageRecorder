// SdController.h

#ifndef _SDCONTROLLER_h
#define _SDCONTROLLER_h


#include <SysCall.h>
#include <sdios.h>
#include <SdFatConfig.h>
#include <SdFat.h>
#include <MinimumSerial.h>
#include <FreeStack.h>
#include <BlockDriver.h>
#include "Settings.h"
#include <SPI.h>
#include "Settings.h"

// Use second SPI port
SPIClass SPI_2(2);
SdFat sd2(&SPI_2);
const uint8_t SD2_CS = PB12;

class SdControllerClass
{
protected:
	SdFat sd;
	File file;

public:
	void init() {

		while (!sd2.begin(SD2_CS, SD_SCK_MHZ(18)))
		{
			Serial.println("SD card not init");
			digitalWrite(PC13, 0);
			delay(300);
			digitalWrite(PC13, 1);
			delay(300);
			//return false;
		}
		// return true;
	}
	void write(byte* buffer) {
		String fileName = "xxx2";
		fileName += ".bvr";
		file = sd2.open(fileName, FILE_WRITE);

		if (file)
		{

			file.write(buffer, BUFFER_SIZE);
			//USART0.send(file);
			file.close();
		}
		else
		{
			Serial.println("error opening file");
			init();
		}
	}

}sdController;

#endif

