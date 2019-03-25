/******************************************************************************
	 * File: PSP.cpp
	 * Description: �������� ��������� �������� ������ Protocol streaming packets(PSP1N)
	 * Created: 21.02.2019
	 * Author: ���������� ����� ���������

******************************************************************************/

#include "PSP.h"

//������� ��������� ���������� ���� � �����
void PSP::setStartBit(byte &value) {
	if (startBit == 0) value &= ~(1 << 7);
	else value |= 1 << 7;
}
//������� ������ ���������� ���� � �����
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
	int remBit = 0; //��� �������� ������� ������������� ���

	//���� ��������� ������ �������, �� �������������� ������ ������
	if (!clearFlag) {
		for (byte i = 0; i < sizeArrayByte; i++)
		{
			arrayByte[i] = 0;
		}
		clearFlag = true;
	}
	//���������� ���� ��������� ��������� �������� �� ����� �� 7 ��� � ������ � ������
	while (remBit > -1)
	{
		free = 7 - (position) % 7; //���������� ��������� ��� � ����� �� ������� �������
		y = (position) / 7; //������� ����� ����� � �������

		//���������� ���������� �� �������� � ������� �������
		remBit = sizeBit - free;
		//���� �������� ���������� � ������� ����
		if (remBit < 0) {
			arrayByte[y] |= value << ~remBit + 1; //�������� �������� � ����, �� ��������� �����			
			position += sizeBit; //����������� ������� ������� �� ������ ����������� ��� ������
			remBit = -1; //��������� ��� ��� ���� �������� �����������
		}
		//���� �������� ������ ��� ��������� ���
		else if (remBit > 0) {
			arrayByte[y] |= value >> remBit; //���������� � ������� ���� ������� ���, ������� ���������� �����
			position += sizeBit - remBit;
			sizeBit = remBit; //�������� ������ ���  �� ���������� �� ������������� ����
		}
		//���� ��������� ��� ������� �� ������� � ��� ��������
		else if (remBit == 0) {
			arrayByte[y] |= value; //��������� �������� � ����
			position += sizeBit;
			remBit = -1; //��������� ��� ��� ���� �������� �����������
		}
		clearStartBit(arrayByte[y]); //������� ��������� ���
	}
	setStartBit(arrayByte[0]); //������������� ��������� ���
}

byte* PSP::popData() {
	position = 0; //���������� ������� �������
	clearFlag = false; //���������� ���� ������������� �������
	return arrayByte; //���������� ������ ����
}


