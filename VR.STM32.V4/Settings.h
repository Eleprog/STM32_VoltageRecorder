// Settings.h

#ifndef _SETTINGS_h
#define _SETTINGS_h

/* START Calculating the size of a matrix byte*/
#define SIZE_BIT_DATE_TIME 32
#define SIZE_BIT_MILLIS 10
#define SIZE_BIT_ADC 12 
#define CHANNELS_ADC 10
#define TOTAL_SIZE_BIT (SIZE_BIT_DATE_TIME+SIZE_BIT_MILLIS+(SIZE_BIT_ADC*CHANNELS_ADC))
#define DIV_TOTAL_BIT (TOTAL_SIZE_BIT/7)
#define REM_TOTAL_BIT (TOTAL_SIZE_BIT%7)
#if REM_TOTAL_BIT>0
#define PACKAGE_SIZE (DIV_TOTAL_BIT+1) //Размер пакета в байтах
#else
#define PACKAGE_SIZE DIV_TOTAL_BIT
#endif // REM_TOTAL_BIT>0  
/* END Calculating the size of a matrix byte*/

#define SD_BUFFER_SIZE 512
#define PACKETS_IN_BUFFER (SD_BUFFER_SIZE/PACKAGE_SIZE) //количество пакетов в буфере sdкарты
#define BUFFER_SIZE (PACKETS_IN_BUFFER*PACKAGE_SIZE) //размер буфера
#define NUMBER_OF_BUFFERS 16 //количество буферов

byte channelsADC[CHANNELS_ADC] = { PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PB0,PB1 };

#define SAMPLING_TIME 2000 //Время дескретизации, мкс
#define TIMER_RESET_COUNTER ((1000000 / SAMPLING_TIME) - 1) //счетчик сброса таймера
#define TIME_FACTOR (1000/(1000000 / SAMPLING_TIME)) //поправочный коэффицент миллисекунд

#endif

