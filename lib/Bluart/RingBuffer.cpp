/*
 * RingBuffer.cpp
 *
 *  Created on: Feb 14, 2021
 *      Author: Marty Hauff
 */

#include "RingBuffer.h"

RingBuffer::RingBuffer(char* newBuff, unsigned int size) {
	buff = newBuff;
	szBuff = size;
	insert = 0;
	extract = 0;
	overflow = 0;
}

int RingBuffer::availableForWrite(void) {
	return (extract > insert ? extract - insert : extract + szBuff - insert);
}	

int RingBuffer::putc(char ch) {
	unsigned int next = insert+1;
	if (next >= szBuff)
		next -= szBuff;
	if (next == extract) {
		overflow++;
		return EOF;
	}
	buff[insert] = ch;
	insert = next;
	return ch;
}

int RingBuffer::puts(const char* str) {
	size_t len = strlen(str);
	if (availableForWrite() < len)
		return EOF;
	return write(str, len);
}

//Returns number of characters written
int RingBuffer::write (uint8_t ch) {
	return (putc(ch) >= 0 ? 1 : 0);
}

int RingBuffer::write (const uint8_t *buffer, size_t size) {
    size_t count = 0;
	while (count < size) {
		putc(*buffer++);
		count++;
	}
	return count;
};

int RingBuffer::available(void)	{
	return (insert >= extract ? insert - extract : insert + szBuff - extract);
}	

int RingBuffer::getc (void) {
	if (extract == insert)
	   return EOF;
	int rtn = buff[extract++];
	if (extract >= szBuff)
	   extract -= szBuff;
	return rtn;
}

char* RingBuffer::getp (void) {
	char* ptr = &buff[extract];
	if (extract == insert)
		return 0;
	if (++extract >= szBuff)
		extract -= szBuff;
	return ptr;
}

int RingBuffer::peek(void) {
	if (extract == insert)
	   return EOF;
	return(buff[extract]);
}

size_t RingBuffer::read(uint8_t *buffer, size_t size) {
    size_t avail = available();
    if (size < avail) {
        avail = size;
    }
    size_t count = 0;
    while(count < avail) {
        *buffer++ = read();
        count++;
    }
    return count;
};
