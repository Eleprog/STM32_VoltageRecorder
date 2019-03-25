/******************************************************************************
	 * File: PSP.cpp
	 * Description: Протокол потоковой передачи данных Protocol streaming packets(PSP1N)
	 * Created: 21.02.2019
	 * Author: Ступеньков Антон Сергеевич

******************************************************************************/

#include "PSP.h"

//Функция установки стартового бита в байте
void PSP::setStartBit(byte &value) {
	if (startBit == 0) value &= ~(1 << 7);
	else value |= 1 << 7;
}
//Функция сброса стартового бита в байте
void PSP::clearStartBit(byte &value) {
	if (startBit == 1) value &= ~(1 << 7);
	else value |= 1 << 7;
}

void PSP::init(byte startBit, byte* arrayByte, byte sizeArrayByte) {
	this->startBit = startBit;
	this->arrayByte = arrayByte;
	this->sizeArrayByte = sizeArrayByte;
}

void PSP::pushData(byte sizeBit, uint32_t value) {
	byte free;
	byte y;
	int remBit = 0; //для хранения остатка недобавленных бит

	//Если добавляем данные впервые, то инициализируем массив нулями
	if (!clearFlag) {
		for (byte i = 0; i < sizeArrayByte; i++)
		{
			arrayByte[i] = 0;
		}
		clearFlag = true;
	}
	//Начинается цикл разбиения входящего значения на куски по 7 бит и записи в массив
	while (remBit > -1)
	{
		free = 7 - (position) % 7; //количество свободных бит в байте на текущей позиции
		y = (position) / 7; //текущий номер байта в массиве

		//Определяем помещается ли значение в текущей позиции
		remBit = sizeBit - free;
		//если значение помещается в текущий байт
		if (remBit < 0) {
			arrayByte[y] |= value << ~remBit + 1; //помещаем значение в байт, со смещением влево			
			position += sizeBit; //увеличиваем текущую позицию на размер добавленных бит данных
			remBit = -1; //указываем что все биты значения поместились
		}
		//если значение больше чем свободных бит
		else if (remBit > 0) {
			arrayByte[y] |= value >> remBit; //записываем в текущий байт столько бит, сколько свободного места
			position += sizeBit - remBit;
			sizeBit = remBit; //изменяем размер бит  на оставшиеся не поместившиеся биты
		}
		//если свободных бит столько же сколько и бит значения
		else if (remBit == 0) {
			arrayByte[y] |= value; //добавляем значение в байт
			position += sizeBit;
			remBit = -1; //указываем что все биты значения поместились
		}
		clearStartBit(arrayByte[y]); //очищаем стартовый бит
	}
	setStartBit(arrayByte[0]); //устанавливаем стартовый бит
}

byte* PSP::popData() {
	position = 0; //сбрасываем текущую позицию
	clearFlag = false; //сбрасываем флаг инициализации массива
	return arrayByte; //возвращаем массив байт
}


