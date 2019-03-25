// Settings.h

#ifndef _SETTINGS_h
#define _SETTINGS_h

/*BEGIN global variable*/
#define CHANNELS_ADC 10 //количество каналов АЦП
#define BIT_ADC 12 //битность АЦП
#define BIT_DATETIME 42 //количество бит дата и время

/*BEGIN global variable*/

byte channelsADC[CHANNELS_ADC] = { PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PB0,PB1 };
//#define PACKAGE_SIZE 21 //размер пакета данных в байтах

/* BEGIN DataMeasure */
#define SD_BUFFER_SIZE 504 //размер буфера данных
#define NUMBER_OF_BUFFER 10 //количество буфферов для данных
/* END DataMeasure */

//#define NUMBER_OF_PACKAGE_IN_BUFFER   SD_BUFFER_SIZE/PACKAGE_SIZE //количество пакетов в буфере
//#define HEADER_BYTE 255 //байт начала посылки
#define SD_PIN 28						//Выбор sd карты
//#define ERROR_SDCARD_PIN 8						//Ошибка чтения sd карты
//#define ERROR_PROGRAMM 9						//Программная ошибка
//#define BIT_RATE 250000
//#define STEP_MEASURE_MS 10 //шаг измерения в мс

#endif

