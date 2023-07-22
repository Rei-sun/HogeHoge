#include "HogeHogeSerial.h"

HogeHogeSerial hogehoge;

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(921600);
  Serial.begin(2000000);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    hogehoge.ReceiveByte(Serial.read());
  }
}
