/*
 * Copyright (c) 2016 Intel Corporation.  All rights reserved.
 * See the bottom of this file for the license terms.
 */

/*
 * Sketch:PeripheralExplorer.ino
 *
 * Description:
 *   This is a Central sketch demonstrating the discovery process
 *   of a Peripheral.  The discovered Attributes are being
 *   display onto the serial output.
 *
 * Notes:
 *
 *  - Expected Peripheral name: LED
 *
 */

#include <CurieBLE.h>

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // initialize the BLE hardware
  BLE.begin();

  Serial.println("BLE Central - Peripheral Explorer");

  // start scanning for peripherals
  BLE.scan();
}

void loop() {
  String str;

  if(Serial.available() > 0){
    // 受信データを読む
    str = Serial.readStringUntil(';');
    if(str.length() > 0){
      // 受信データを書く。
      Serial.print("I received: ");
      Serial.println(str);
      // check if a peripheral has been discovered
      BLEDevice peripheral = BLE.available();
      if(peripheral){
        // discovered a peripheral, print out address, local name, and advertised service
        Serial.print("Found address=");
        Serial.print(peripheral.address());
        Serial.print(",localName=");
        Serial.print(peripheral.localName());
        Serial.print(",advertisedServiceUuid=");
        Serial.print(peripheral.advertisedServiceUuid());
        Serial.println();
        if (peripheral.localName() == str) { //"02010012"
          Serial.println("localName match");
          // stop scanning
          BLE.stopScan();
          explorerPeripheral(peripheral);
        }else{
          Serial.println("localName mismatch");
          BLE.stopScan();
        }
        BLE.scan();
      }
    }
    Serial.println("loop end");
  }
}

void explorerPeripheral(BLEDevice peripheral) {
  // connect to the peripheral
  Serial.println("Connecting ...");

  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect!");
    return;
  }

#if 0
  // discover peripheral attributes
  Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) {
    Serial.println("Attributes discovered");
  } else {
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }
#endif
#if 1
  if(peripheral.discoverAttributes()){
    Serial.println("attributes discovered");
  }
  BLECharacteristic authBleCharacteristic = peripheral.characteristic("ebafb2f0-0e0f-40a2-a84f-e2f098dc13c3");
  if (authBleCharacteristic.canRead()) {
    Serial.println("canRead");
  }
  if (authBleCharacteristic.canWrite()) {
    Serial.println("canWrite");
  }
  if (authBleCharacteristic.canSubscribe()) {
    Serial.println("canSubscribe");
  }
  if (authBleCharacteristic.subscribe()) {
    Serial.println("subscribed");
  }
  int i = 0;
  while(peripheral.connected()) {
    if (authBleCharacteristic.setValue("00000000-0000-0000-0000-000000000000")) {
      Serial.println("write success");
      if(peripheral.discoverAttributes()){
        Serial.println("attributes discovered");
      }
    } else {
      Serial.println("write fail");
    }
    i++;
    delay(3000);
  }
#endif

  // read and print device name of peripheral
  Serial.println();
  Serial.print("Device name: ");
  Serial.println(peripheral.deviceName());

  // loop the services of the peripheral and explore each
  for (int i = 0; i < peripheral.serviceCount(); i++) {
    BLEService service = peripheral.service(i);

    exploreService(service);
  }

  Serial.println();

  // we are done exploring, disconnect
  Serial.println("Disconnecting ...");
  peripheral.disconnect();
  Serial.println("Disconnected");
}

void exploreService(BLEService service) {
  // print the UUID of the service
  Serial.print("Service ");
  Serial.println(service.uuid());

  // loop the characteristics of the service and explore each
  for (int i = 0; i < service.characteristicCount(); i++) {
    BLECharacteristic characteristic = service.characteristic(i);

    exploreCharacteristic(characteristic);
  }
}

void exploreCharacteristic(BLECharacteristic characteristic) {
  // print the UUID and properies of the characteristic
  Serial.print("\tCharacteristic ");
  Serial.print(characteristic.uuid());
  Serial.print(", properties 0x");
  Serial.print(characteristic.properties());

  // check if the characteristic is readable
  if (characteristic.canRead()) {
    // read the characteristic value
    characteristic.read();

    if (characteristic.valueLength() > 0)
    {
      // print out the value of the characteristic
      Serial.print(", value 0x");
      printData(characteristic.value(), characteristic.valueLength());
    }
  }
  Serial.println();

  // loop the descriptors of the characteristic and explore each
  for (int i = 0; i < characteristic.descriptorCount(); i++) {
    BLEDescriptor descriptor = characteristic.descriptor(i);

    exploreDescriptor(descriptor);
  }
}

void exploreDescriptor(BLEDescriptor descriptor) {
  // print the UUID of the descriptor
  Serial.print("\t\tDescriptor ");
  Serial.print(descriptor.uuid());

  // read the descriptor value
  descriptor.read();

  // print out the value of the descriptor
  Serial.print(", value 0x");
  printData(descriptor.value(), descriptor.valueLength());

  Serial.println();
}

void printData(const unsigned char data[], int length) {
  for (int i = 0; i < length; i++) {
    unsigned char b = data[i];

    if (b < 16) {
      Serial.print("0");
    }

    Serial.print(b, HEX);
  }
}


/*
  Arduino BLE Central peripheral explorer example
  Copyright (c) 2016 Arduino LLC. All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/


