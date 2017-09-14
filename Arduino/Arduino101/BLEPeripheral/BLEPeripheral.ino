// This is a Peripheral sketch
// https://garchiving.com/how-to-curieble-of-genuino-arduino-101/
#include "CurieBLE.h"

BLEPeripheral blePeripheral;
BLEService CurieBLEService("19B10010-E8F2-537E-4F6C-D104768A1214");
BLECharacteristic axisCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite, 20);

void setup() {
  Serial.begin(115200);
  blePeripheral.setLocalName("CurieBLE");
  blePeripheral.setAdvertisedServiceUuid(CurieBLEService.uuid());

  blePeripheral.addAttribute(CurieBLEService);
  blePeripheral.addAttribute(axisCharacteristic);
  axisCharacteristic.setEventHandler(BLEWritten, axisCharacteristicWritten);

  blePeripheral.begin();
}

void loop() {
  BLECentral central = blePeripheral.central();

  if (central) {
    digitalWrite(13, HIGH);

    while (central.connected()) {
  //"ここに処理を記述します"
#if 1
  //データ送信のスケッチ例
  char s[7];
  char t[7];
  char u[7];
  char buf[20];
  float pitch = 123.1;
  float roll  = 10.12345;
  float yaw   = -10;

  dtostrf(pitch, 1, 1, s);
  dtostrf(roll, 1, 1, t);
  dtostrf(yaw, 1, 1, u);

  sprintf(buf, "%s,%s,%s", s, t, u );
  int bufSize = strlen(buf);
  axisCharacteristic.setValue((unsigned char*)buf, bufSize);
#else
  //データ受信のスケッチ例
      if (axisCharacteristic.written()) {
        if (axisCharacteristic.value()) {
          for (int i = 0; i < 20; i++) {
            buf[i] = *(axisCharacteristic.value() + i);
          }
          Serial.println(buf);
        }
      }
#endif
    }
    digitalWrite(13, LOW);
  }
}

//データ受信のイベントハンドラ例
void axisCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic) {
  char buf[20];
  for (int i = 0; i < 20; i++) {
    buf[i] = *(axisCharacteristic.value() + i);
  }
  Serial.println(buf);
}

