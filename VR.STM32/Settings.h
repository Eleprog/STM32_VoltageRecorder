// Settings.h

#ifndef _SETTINGS_h
#define _SETTINGS_h

/*BEGIN global variable*/
#define CHANNELS_ADC 10 //���������� ������� ���
#define BIT_ADC 12 //�������� ���
#define BIT_DATETIME 42 //���������� ��� ���� � �����

/*BEGIN global variable*/

byte channelsADC[CHANNELS_ADC] = { PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PB0,PB1 };
//#define PACKAGE_SIZE 21 //������ ������ ������ � ������

/* BEGIN DataMeasure */
#define SD_BUFFER_SIZE 504 //������ ������ ������
#define NUMBER_OF_BUFFER 10 //���������� �������� ��� ������
/* END DataMeasure */

//#define NUMBER_OF_PACKAGE_IN_BUFFER   SD_BUFFER_SIZE/PACKAGE_SIZE //���������� ������� � ������
//#define HEADER_BYTE 255 //���� ������ �������
#define SD_PIN 28						//����� sd �����
//#define ERROR_SDCARD_PIN 8						//������ ������ sd �����
//#define ERROR_PROGRAMM 9						//����������� ������
//#define BIT_RATE 250000
//#define STEP_MEASURE_MS 10 //��� ��������� � ��

#endif

