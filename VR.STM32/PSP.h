/******************************************************************************
	 * File: PSP.h
	 * Description: �������� ��������� �������� ������ Protocol streaming packets(PSP1N)
	 * Created: 21.02.2019
	 * Author: ���������� ����� ���������

��� ���� �����:
��� �������� ������ ������� ������ ����������������� �� ������� �����.

�������� ���������:
����������� ����� �������������� ������� � ������.
������ ��� ������� ����� ��������� �� ������� ���������� ����.
��������� ���� ��� ��������� ��� ������.
������������������ � ������� ������ �������� �������.
������ ���������� ��������.

������:
����� �� ���� ����, ��� 7 - ����� ���, 6,5,4,3,2,1,0 - ���� ������.
��������� ��� ����� �������.
�������� ����� ���������� �� ��� ��������� � ��� � ������ ������.
������ 10 ���, ������ 8 ���, ����� 18 ��� ������.
18 ��� / 7 ��� = 2,57..., ��������� � ������� ������� �� 3.
������ ��� ����� �������� 3 ����� ��� ������:
 7 6 5 4 3 2 1 0     7 6 5 4 3 2 1 0     7 6 5 4 3 2 1 0
|1|x|x|x|x|x|x|x|   |0|x|x|x|x|x|x|x|   |0|x|x|x|x|x|x|x|
  |---------10 ���----------|---------8 ���-------|-�������������� ����-
  |-----------------------18 ���------------------|-�������������� ����-

******************************************************************************/

#ifndef _PSP_h
#define _PSP_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define DATA_BIT 7 //������ ������ � �����

class PSP
{
public:
	/*
	������� ������������� init:
	startBit - ��������� ��� 0 ��� 1
	*arrayByte - ��������� �� ������ ���� ������
	sizeArrayByte - ���������� ���� � ������
	*/
	void init(byte startBit, byte* arrayByte, byte sizeArrayByte);
	/*
	������� ���������� ������ � ����� pushData:
	sizeBit - ������ ������ � �����
	value - ������������ �������� (������ ��������� � ��������� ����� ������ � �����)
	*/
	void pushData(byte sizeBit, uint32_t value);
	/*
	������� ��������� ������ � �������������� ������ popData:
	return ����� ������ � ������� ����.
	*/
	byte* popData();

protected:
	byte startBit; //��������� ���
	byte* arrayByte; //������ ���� ������
	byte sizeArrayByte; //������ ������� ������
	byte position = 0; //������� ������� � ������
	bool clearFlag = false; //���� ������������� ������� 
	void setStartBit(byte &value); //��������� ���������� ���� � �����
	void clearStartBit(byte &value); //����� ���������� ���� � �����
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

