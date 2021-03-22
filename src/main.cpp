#include <Arduino.h>
#include <HardwareSerial.h>
#include <Bluart.h>

#define DEBUG_PRINT(...)		  do {Blerial.print(__VA_ARGS__); Serial.print(__VA_ARGS__); } while(0)
#define DEBUG_PRINTLN(...)		do {Blerial.println(__VA_ARGS__); Serial.println(__VA_ARGS__); } while(0)

void setup() {
  Serial.begin(9600);             //Initialize the (hard) connected terminal
  BLEDevice::init("Blerial");     //Initialize the BLE Server
  Blerial.begin();                //begin the Bluart service

  delay(5000);    //Delay so have time to connect external BLE terminal app and see the "Hello World"
}

//ping-pong characters from either terminal to the other
void loop() {
  char ch;
  DEBUG_PRINTLN("Hello World");
  for(;;){
    if (Serial.available()) {
      ch = Serial.read();
      DEBUG_PRINT(ch);
    }
    if (Blerial.available()) {
      ch = Blerial.read();
      DEBUG_PRINT(ch);
    }
  }
}