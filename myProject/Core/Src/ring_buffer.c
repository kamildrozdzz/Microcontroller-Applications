/* Includes ------------------------------------------------------------------*/
#include <assert.h>
#include "ring_buffer.h"


bool RingBuffer_Init(RingBuffer *ringBuffer, char *dataBuffer, size_t dataBufferSize) 
{
	assert(ringBuffer);
	assert(dataBuffer);
	assert(dataBufferSize > 0);
	
	if ((ringBuffer) && (dataBuffer) && (dataBufferSize > 0)) {
		ringBuffer->buffer = dataBuffer;
		ringBuffer->capacity = dataBufferSize;
		ringBuffer->head = 0;
		ringBuffer->tail = 0;
		ringBuffer->size = 0;
		return true;
	}
	
	return false;
}

bool RingBuffer_Clear(RingBuffer *ringBuffer)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
		ringBuffer->head = 0;
		ringBuffer->tail = 0;
		ringBuffer->size = 0;
		return true;
	}
	return false;
}

bool RingBuffer_IsEmpty(const RingBuffer *ringBuffer)
{
  assert(ringBuffer);	
	if(ringBuffer){
		return (ringBuffer->size == 0);
	}
	
	return true;
}

size_t RingBuffer_GetLen(const RingBuffer *ringBuffer)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
		return ringBuffer->size;
	}
	return 0;
	
}

size_t RingBuffer_GetCapacity(const RingBuffer *ringBuffer)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
		return ringBuffer->capacity;
	}
	return 0;	
}


bool RingBuffer_PutChar(RingBuffer *ringBuffer, char c)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
		//Check if buffer is full
		if(ringBuffer->size >= ringBuffer->capacity){
			return false;
		}

		//Aadd the character to the buffer
		ringBuffer->buffer[ringBuffer->head] = c;

		//Move head position
		ringBuffer->head = (ringBuffer->head + 1) % ringBuffer->capacity;
		ringBuffer->size++;

		return true;
	}
	return false;
}

bool RingBuffer_GetChar(RingBuffer *ringBuffer, char *c)
{
	assert(ringBuffer);
	assert(c);
	
  if ((ringBuffer) && (c)) {
		//Check if buffer is empty
		if(ringBuffer->size == 0){
			return false;
		}
		
		//Get the character from the buffer
		*c = ringBuffer->buffer[ringBuffer->tail];

		//Move tail position
		ringBuffer->tail = (ringBuffer->tail + 1) % ringBuffer->capacity;

		//Decrease size
		ringBuffer->size--;

		return true;
	}
	return false;
}
