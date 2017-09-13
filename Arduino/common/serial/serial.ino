void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  String str;
  if(Serial.available() > 0){
    // 受信データを読む
    str = Serial.readStringUntil(';');
    if(str.length() > 0){
      // 受信データを書く。
      Serial.print("I received: ");
      Serial.println(str);
    }
  }
}
