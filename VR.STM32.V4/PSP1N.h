/******************************************************************************
	 * File: PSP1N.h
	 * Description: Protocol streaming packets(PSP1N)
	 * Created: 30.03.2019
	 * Author: Stupenkov Anton Sergeevich

******************************************************************************/

#ifndef _PSP1N_h
#define _PSP1N_h
#define DATA_BITS 7 //number of data bits per byte
//Start bit 
typedef enum StartBit
{
	ZERO,
	ONE
}StartBit;

class PSP {
public:
	template<uint16_t sizeCodedData>
	void init(StartBit startBit, byte(&arrayData)[sizeCodedData]) {
		this->startBit = startBit;
		codedData = arrayData;
		this->sizeCodedData = sizeCodedData;
	}
	void push(byte size, uint32_t value) {
		if (!isClearArray)
		{
			for (size_t i = 0; i < sizeCodedData; i++)
			{
				codedData[i] = 0;
			}
			isClearArray = true;
		}
		int _size = size; //size of remaining bits not placed
		byte x = 0;
		uint16_t y = 0;
		while (_size > 0)
		{
			x = position % DATA_BITS; //
			y = position / DATA_BITS;
			byte freeBits = DATA_BITS - x; //свободных бит в текущем байте
			byte offsetRight = size - _size; //смещение вправо

			codedData[y] |= (byte)(value >> offsetRight << x);
			_size -= freeBits;

			if (startBit == ONE)
				codedData[y] &= ~(1 << 7);
			else codedData[y] |= 1 << 7;

			if (_size <= 0)
			{
				position += freeBits + _size;
				break;
			}
			else
			{
				position += freeBits;
			}
		}
		if (startBit == ZERO)
			codedData[0] &= ~(1 << 7);
		else codedData[0] |= 1 << 7;
	}
	byte* getData() {
		isClearArray = false;
		position = 0;
		return codedData;
	}
private:
	StartBit startBit;
	byte* codedData;
	uint16_t sizeCodedData;
	uint16_t position;
	bool isClearArray = false;
};



//Data unit
struct DataUnit
{
	DataUnit(byte size, uint32_t value = 0) {
		this->size = size;
		this->value = value;
	}
	uint32_t value = 0;
	byte getSize() {
		return size;
	}
private:
	byte size = 0; //data size in bits
};

//Data packet
class PackagePSP
{
public:
	//Package data
	DataUnit* getItem() {
		return item;
	}
	//The amount of data in the package
	uint16_t getItemCount() {
		return itemCount;
	}
	//Packet start bit
	StartBit getStartBit() {
		return startBit;
	}
	//Constructor
	template <uint16_t sizeMatrix>
	PackagePSP(StartBit startBit, DataUnit(&dataUnits)[sizeMatrix]) {
		this->startBit = startBit;
		this->item = dataUnits;
		this->itemCount = sizeMatrix;
	}
private:
	uint16_t itemCount;
	StartBit startBit;
	DataUnit* item;
};

//Class for encoding and decoding packets
class PSP1N
{
public:
	/*Encode package
		packagePSP - encoding package
		matrix - put the result of the encoded byte array */
	template <uint16_t sizeMatrix>
	static void encode(PackagePSP& packagePSP, byte(&matrix)[sizeMatrix]) {
		for (size_t i = 0; i < sizeMatrix; i++)
		{
			matrix[i] = 0;
		}
		int positionPush = 0;

		for (uint16_t i = 0; i < packagePSP.getItemCount(); i++)
		{
			int size = packagePSP.getItem()[i].getSize(); //size of remaining bits not placed
			byte x = 0;
			uint16_t y = 0;
			while (size > 0)
			{
				x = positionPush % DATA_BITS; //
				y = positionPush / DATA_BITS;
				byte freeBits = DATA_BITS - x; //свободных бит в текущем байте
				byte offsetRight = packagePSP.getItem()[i].getSize() - size; //смещение вправо

				matrix[y] |= (byte)(packagePSP.getItem()[i].value >> offsetRight << x);
				size -= freeBits;

				if (packagePSP.getStartBit() == ONE)
					matrix[y] &= ~(1 << 7);
				else matrix[y] |= 1 << 7;

				if (size <= 0)
				{
					positionPush += freeBits + size;
					break;
				}
				else
				{
					positionPush += freeBits;
				}
			}

			if (packagePSP.getStartBit() == ZERO)
				matrix[0] &= ~(1 << 7);
			else matrix[0] |= 1 << 7;
		}
	}

	/*Decode package
		dataByte - data byte encoded
		packagePSP - put the result of decoding data bytes
		matrix - intermediate byte array */
	template <uint16_t sizeMatrix>
	void decode(int dataByte, PackagePSP &packagePSP, byte(&matrix)[sizeMatrix]) {
		if (dataByte == -1) return;
		matrix[countData] = dataByte;
		if (matrix[countData] & (1 << 7) == packagePSP.getStartBit()) {
			matrix[0] = matrix[countData];
			countData = 1;
			return;
		}
		countData++;
		if (countData >= sizeMatrix)
		{
			int position = 0;
			for (size_t i = 0; i < packagePSP.getItemCount(); i++)
			{
				packagePSP.getItem()[i].value = 0;
				int size = packagePSP.getItem()[i].getSize();
				while (size > 0)
				{
					byte x = position % DATA_BITS; //смещение вправо
					uint16_t y = position / DATA_BITS;
					int freeBits = 8 - (x + size); //свободных бит в текущем байте
					if (freeBits <= 0) freeBits = 1;
					byte temp = matrix[y] << freeBits;
					uint32_t temp2 = temp >> (freeBits + x);
					packagePSP.getItem()[i].value |= temp2 << (packagePSP.getItem()[i].getSize() - size);
					byte bit = DATA_BITS - (freeBits - 1) - x;
					position += bit;
					size -= bit;
				}
			}
			countData = 0;
		}
	}
private:
	uint16_t countData = 0;
};


#endif

