/*
 * Bluart.cpp
 *
 *  Created on: Mar 23, 2021
 *      Author: Marty Hauff
 */
#include <Arduino.h>

#include "HardwareSerial.h"

#include <Bluart.h>
#include "esp_task.h"

/*
 *  For a BLE Uart Terminal device:
 *  - iOS: https://apps.apple.com/au/app/bluefruit-connect/id830125974
 *  - Android: https://play.google.com/store/apps/details?id=com.adafruit.bluefruit.le.connect 
 */

  

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

//Can only have one instance of Bluart
int Bluart::instances = 0;

//Define static Tx/Rx buffers
char txBuff[TX_BUFF_SZ];
char rxBuff[RX_BUFF_SZ];

Bluart Blerial;

void BlerialLoopTask(void* param) {
  for(;;) {
    Blerial.loop();
    vTaskDelay(1);
  }
}

Bluart::Bluart(void) : 
  RX(rxBuff, RX_BUFF_SZ),
  TX(txBuff, TX_BUFF_SZ) {
    if (instances > 0) return;
  instances++;
  pServer = NULL;
  deviceConnected = false;
  oldDeviceConnected = false;
}

void Bluart::begin(void) {
  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new BluartServerCallbacks());

  // Create the BLE Service
  pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
										CHARACTERISTIC_UUID_TX,
										BLECharacteristic::PROPERTY_NOTIFY
									);
                      
  pTxCharacteristic->addDescriptor(new BLE2902());

  pRxCharacteristic = pService->createCharacteristic(
											 CHARACTERISTIC_UUID_RX,
											BLECharacteristic::PROPERTY_WRITE
										);

  pRxCharacteristic->setCallbacks(new BluartRxCharacteristicCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
//  Serial.println("Starting Blerial");
  xTaskCreate(BlerialLoopTask, "Blerial Loop", 10000, NULL, 3, NULL);
}

void Bluart::loop(void) {
    static uint8_t txCharBuff[TX_BUFF_SZ];
    size_t len = TX.available();
    for (int i = 0; i < len; i++) {
      txCharBuff[i] = TX.getc();
    }

    if (deviceConnected && len) {
      pTxCharacteristic->setValue(txCharBuff, len);
      pTxCharacteristic->notify();
      vTaskDelay(10); // bluetooth stack will go into congestion, if too many packets are sent
    }

    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        vTaskDelay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
		// do stuff here on connecting
        Serial.println("Connection established");
        oldDeviceConnected = deviceConnected;
    }
}

int Bluart::read(void) {
  if (available()) {
    return RX.getc();
  }
  return EOF;
}

void BluartServerCallbacks::onConnect(BLEServer* pServer)     { Blerial.setDeviceConnected(true); }
void BluartServerCallbacks::onDisconnect(BLEServer* pServer)  { Blerial.setDeviceConnected(false); }

void BluartRxCharacteristicCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
  std::string rxValue = pCharacteristic->getValue();
  if (rxValue.length() > 0) {
    for (int i = 0; i < rxValue.length(); i++) {
      Blerial.rx_write(rxValue[i]);
    }
  }
};
