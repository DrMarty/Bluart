# Bluart
Serial IO via a BLE connection for PlatformIO / Arduino / ESP32 environment

## Getting Started
### Prerequisites
You'll need a working VSCode environment with the following extensions
* PlatformIO (platformio.platformio-ide)
* Espressif IDF (espressif.esp-idf-extension)

BLE terminal Apps can be found at:
* iOS: https://apps.apple.com/au/app/bluefruit-connect/id830125974
* Android: https://play.google.com/store/apps/details?id=com.adafruit.bluefruit.le.connect 

You'll also need some ESP32 hardware (with BLE capabilities) to run it on.
I sourced an ESP32-WROOM-32D variant from Aliexpress: https://www.aliexpress.com/item/4001104664948.html

### Simplest
1. Clone this entire repository to a local drive and open the folder in Visual Studio Code.
2. Build, Upload & Monitor the project from within VSCode
3. Run one of the BLE apps listed above and open a UART terminal interface.
4. Entering text in either the VSCode Terminal or the BLE terminal should be mirrored to the other device

### Include in your own project
1. Copy the lib/Bluart folder (and its contents) from this repository into your PlatformIO project's lib folder.
2. Add the following code to your top level source (i.e. main.cpp)
```c++
#include <Bluart.h>               //Including Bluart.h will add the 'Blerial' instance
...
void setup() {
  ...
  BLEDevice::init("Blerial");     //Initialize the BLE Server
  Blerial.begin();                //begin the Bluart service
  ...
}
```
You can then read/write to the Blerial (BLE Serial) interface using Blerial.read() and Blerial.write() respectively
```c++
  ...
  char ch = Blerial.read();
  char* str[] = "This is a message";
  Blerial.write(str);
  ...
```
