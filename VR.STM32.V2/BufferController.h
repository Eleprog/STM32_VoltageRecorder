// BufferController.h

#ifndef _BUFFERCONTROLLER_h
#define _BUFFERCONTROLLER_h
#include "Buffer.h"


class BufferController
{
 protected:
	 Buffer buffers[NUMBER_OF_BUFFER];
	 byte indxIn = 0;
	 byte indxOut = 0;
	 bool isBufferReady = false;
	 void pushBuffer() {
		 indxIn++;
		 if (indxIn >= NUMBER_OF_BUFFER) indxIn = 0;
	 }
 public:

	 byte* popBuffer() {
		 byte* buff = buffers[indxOut].getData();
		 indxOut++;
		 if (indxOut >= NUMBER_OF_BUFFER) indxOut = 0;
		 return buff;
	 }
	 
	 bool addPackage(Package package) {
		 if (buffers[indxIn].addPackage(package))
		 {
			 pushBuffer();
			 isBufferReady = true;
		 }
		 return isBufferReady;
	 }

	 void clearFlagBufferReady() {
		 isBufferReady = false;
	 }

	 byte getCount() {
		 byte count = 0;
		 if (indxIn<indxOut)
		 {
			 count = NUMBER_OF_BUFFER + indxIn - indxOut;
		 }
		 else {
			 count = indxIn - indxOut;
		 }
		 return count;
	 }
};

#endif

