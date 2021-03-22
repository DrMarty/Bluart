/*
 * Bluart.h
 *
 *  Created on: Mar 23, 2021
 *      Author: Marty Hauff
 */
#ifndef BLUART_H
#define BLUART_H

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#include <inttypes.h>

#include "Stream.h"

#include <RingBuffer.h>

//#define BLUART_MIRROR      Serial

//void BLE_Uart_init(void);
//void BLE_UART_loop(void);

#define TX_BUFF_SZ      128
#define RX_BUFF_SZ      128

class BluartServerCallbacks: public BLEServerCallbacks {
 public:
   void onConnect(BLEServer* pServer);
   void onDisconnect(BLEServer* pServer);
};

class BluartRxCharacteristicCallbacks: public BLECharacteristicCallbacks {
 public:
   void onWrite(BLECharacteristic *pCharacteristic);
};

class Bluart: public Stream {
 public:
   Bluart(void);
   void begin(void);
   void loop(void);       //background multitasking processing loop

   bool isDeviceConnected(void)           { return deviceConnected; };
   void setDeviceConnected(bool onoff)    { deviceConnected = onoff; };

   size_t getTxBuffSize(void)             { return TX.getBuffSize(); };
   size_t getRxBuffSize(void)             { return RX.getBuffSize(); };

   int putc (char ch)                     { return TX.putc(ch); };
   int puts (const char* str)             { return TX.puts(str); };
   int getc (void)                        { return RX.getc(); };

   int available(void)                    { return RX.available(); };
   int read(void);
   int peek(void)                         { return RX.peek(); };

   void flush(void) { }; //No way to force data out the BLE interface so its a dummy function

   size_t write(uint8_t ch)                                 { return TX.write(ch); };
   size_t write(const uint8_t *buffer, size_t size)         { return TX.write(buffer, size); };
   inline size_t write(const char * buffer, size_t size)    { return write((uint8_t*) buffer, size); }
   inline size_t write(const char * s)                      { return write((uint8_t*) s, strlen(s)); }
   inline size_t write(unsigned long n)                     { return write((uint8_t) n); }
   inline size_t write(long n)                              { return write((uint8_t) n); }
   inline size_t write(unsigned int n)                      { return write((uint8_t) n); }
   inline size_t write(int n)                               { return write((uint8_t) n); }

   int availableForWrite(void)                              { return TX.availableForWrite(); };

   friend void BluartRxCharacteristicCallbacks::onWrite(BLECharacteristic *pCharacteristic);

 private:
   BLEServer *pServer;
   BLEService *pService;
   BLECharacteristic *pTxCharacteristic;
   BLECharacteristic *pRxCharacteristic;

   RingBuffer RX;
   RingBuffer TX;
   static int instances;
   bool deviceConnected;
   bool oldDeviceConnected;
   
   size_t rx_write(uint8_t ch) { return RX.write(ch); };
};

//Reference the single instance of Bluart
extern Bluart Blerial;

#endif //BLUART_H