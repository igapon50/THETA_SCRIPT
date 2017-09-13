// This is a Central sketch
//https://www.arduino.cc/en/Reference/BLECentralConstructor
#include <CurieBLE.h>

// variables for button
const int buttonPin = 2;
int oldButtonState = LOW;


void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // configure the button pin as input
  pinMode(buttonPin, INPUT);

  // initialize the BLE hardware
  BLE.begin();

  Serial.println("BLE Central - scanForUuid? [f74d1ba5-bc52-4f10-99c0-85e785ebe43f;]");
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

      // start scanning for peripherals
      //https://developers.theta360.com/ja/docs/bluetooth_reference/
      BLE.scanForUuid(str);
      // check if a peripheral has been discovered
      BLEDevice peripheral = BLE.available();

      if (peripheral) {
        // discovered a peripheral, print out address, local name, and advertised service
        Serial.print("Found ");
        Serial.print(peripheral.address());
        Serial.print(" '");
        Serial.print(peripheral.localName());
        Serial.print("' ");
        Serial.print(peripheral.advertisedServiceUuid());
        Serial.println();

        // stop scanning
        BLE.stopScan();

        controlLed(peripheral);
      }
    }
  }
}

void controlLed(BLEDevice peripheral) {
  // connect to the peripheral
  Serial.println("Connecting ...");

  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) {
    Serial.println("Attributes discovered");
  } else {
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }

  // retrieve the LED characteristic
  BLECharacteristic ledCharacteristic = peripheral.characteristic("f74d1ba5-bc52-4f10-99c0-85e785ebe43f");

  if (!ledCharacteristic) {
    Serial.println("Peripheral does not have LED characteristic!");
    peripheral.disconnect();
    return;
  } else if (!ledCharacteristic.canWrite()) {
    Serial.println("Peripheral does not have a writable LED characteristic!");
    peripheral.disconnect();
    return;
  }

  while (peripheral.connected()) {
    // while the peripheral is connection

    // read the button pin
    int buttonState = digitalRead(buttonPin);

    if (oldButtonState != buttonState) {
      // button changed
      oldButtonState = buttonState;

      if (buttonState) {
        Serial.println("button pressed");

        // button is pressed, write 0x01 to turn the LED on
        ledCharacteristic.writeByte(0x01);
      } else {
        Serial.println("button released");

        // button is released, write 0x00 to turn the LED of
        ledCharacteristic.writeByte(0x00);
      }
    }
  }

  Serial.println("Peripheral disconnected");
}
