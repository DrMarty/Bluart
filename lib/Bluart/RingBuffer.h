/*
 * RingBuffer.h
 *
 *  Created on: Feb 14, 2021
 *      Author: Marty Hauff
 */

#ifndef INC_RINGBUFFER_H_
#define INC_RINGBUFFER_H_

#include <inttypes.h>
#include <stddef.h>
#include <string.h>

#ifndef EOF
#define EOF				-1
#endif
#define RB_OVERFLOW		-2

class RingBuffer {
 public:
    RingBuffer (char* newBuff, unsigned int size);

    int availableForWrite(void);    //returns available size
    int putc (char ch);      //returns the written char
    int puts(const char* str);
    int write (uint8_t ch);  //returns the number of chars written
    int write (const uint8_t *buffer, size_t size);
    inline size_t write(const char * buffer, size_t size)    { return write((uint8_t*) buffer, size); }
    inline size_t write(const char * s)                      { return write((uint8_t*) s, strlen(s)); }
    inline size_t write(unsigned long n)                     { return write((uint8_t) n); }
    inline size_t write(long n)                              { return write((uint8_t) n); }
    inline size_t write(unsigned int n)                      { return write((uint8_t) n); }
    inline size_t write(int n)                               { return write((uint8_t) n); }

    int available(void);
    int peek(void);
    int getc (void);
    char* getp(void);				//Same as getc but returns a pointer to the next character rather than the character itself
    inline int read(void) { return getc(); };
    size_t read(uint8_t *buffer, size_t size);
    inline size_t read(char *buffer, size_t size) {
      return read((uint8_t*) buffer, size);
    }

   char* getBuff(void) 				    { return buff; };
   unsigned int getBuffSize(void) { return szBuff; };
   unsigned int getOverflow(void)	{ return overflow; };

 protected:
   char* buff;
   unsigned int szBuff;

 private:
   unsigned int insert;
   unsigned int extract;
   unsigned int overflow;			//Count the number of times an overflow condition occured
};


#endif /* INC_RINGBUFFER_H_ */
