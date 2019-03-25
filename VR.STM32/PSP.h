/******************************************************************************
	 * File: PSP.h
	 * Description: Протокол потоковой передачи данных Protocol streaming packets(PSP1N)
	 * Created: 21.02.2019
	 * Author: Ступеньков Антон Сергеевич

Для чего нужен:
Для передача данных сжатого объема микроконтроллером во внешнюю среду.

Описание протокола:
Формируется пакет фиксированного размера в байтах.
Первый бит каждого байта отводится на признак стартового бита.
Остальные семь бит отводятся под данные.
Последовательность и размеры данных известны заранее.
Данные передаются побайтно.

Пример:
Пакет из трех байт, где 7 - старт бит, 6,5,4,3,2,1,0 - биты данных.
Стартовый бит будет единица.
Допустим нужно передавать по два измерения с АЦП в каждом пакете.
Первое 10 бит, второе 8 бит, всего 18 бит данных.
18 бит / 7 бит = 2,57..., округляем в большую сторону до 3.
Значит нам нужно выделить 3 байта для пакета:
 7 6 5 4 3 2 1 0     7 6 5 4 3 2 1 0     7 6 5 4 3 2 1 0
|1|x|x|x|x|x|x|x|   |0|x|x|x|x|x|x|x|   |0|x|x|x|x|x|x|x|
  |---------10 бит----------|---------8 бит-------|-неиспользуемые биты-
  |-----------------------18 бит------------------|-неиспользуемые биты-

******************************************************************************/

#ifndef _PSP_h
#define _PSP_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define DATA_BIT 7 //размер данных в байте

class PSP
{
public:
	/*
	Функция инициализации init:
	startBit - стартовый бит 0 или 1
	*arrayByte - указатель на массив байт пакета
	sizeArrayByte - количество байт в пакете
	*/
	void init(byte startBit, byte* arrayByte, byte sizeArrayByte);
	/*
	Функция добавления данных в пакет pushData:
	sizeBit - размер данных в битах
	value - передаваемое значение (должно умещаться в указанный ранее размер в битах)
	*/
	void pushData(byte sizeBit, uint32_t value);
	/*
	Функция получения данных в сформированном пакете popData:
	return пакет данных в массиве байт.
	*/
	byte* popData();

protected:
	byte startBit; //стартовый бит
	byte* arrayByte; //массив байт пакета
	byte sizeArrayByte; //размер массива пакета
	byte position = 0; //текущая позиция в пакете
	bool clearFlag = false; //флаг инициализации массива 
	void setStartBit(byte &value); //установка стартового бита в байте
	void clearStartBit(byte &value); //сброс стартового бита в байте
};

class DataUnit
{
public:
	DataUnit(byte size, uint32_t value = 0)
	{
		
	}
	uint32_t getValue() {
		return value;
	}
	void setValue(uint32_t value) {
		this->value = value;
	}
	byte getSize() {
		return size;
	}
	void setSize(byte size) {
		if (size > 32)this->size = 32;
		else this->size = size;
	}
protected:
	byte size = 32;
	uint32_t value = 0;
};

#endif

