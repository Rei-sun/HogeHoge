#include "HogeHogeSerial.h"
#include "EncoderMod.h"
#include "SensorMod.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define BNO055_SAMPLERATE_DELAY_MS (100)

hw_timer_t * timer = NULL;

EncoderMod encoderModule(1);
SensorMod sensorModule(1);
Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28);
HogeHogeSerial hogehoge(encoderModule, sensorModule);

bool BNO055Begin = false;
volatile bool interrupt = false;

int gpio_pin = 32;
int adc_pin = 34;

void IRAM_ATTR onTimer() {
  interrupt = true;
}

void setup() {
  //-----------------------------------
  // シリアル 設定
  //-----------------------------------
  Serial.begin(2000000);
  while (!Serial) delay(10);

  //-----------------------------------
  // GPIO 設定
  //-----------------------------------
  pinMode(gpio_pin, INPUT_PULLDOWN);

  //-----------------------------------
  // ADC 設定
  //-----------------------------------
  // 

  //-----------------------------------
  // BNO055 設定
  //-----------------------------------
  BNO055Begin = bno.begin();

  //-----------------------------------
  // タイマー 設定
  //-----------------------------------
  // ペリフェラル周波数 80MHz / 80 = 1MHz
  timer = timerBegin(0, 80, true);
  // 関数登録
  timerAttachInterrupt(timer, &onTimer, true);
  // 1MHz / 10000 = 100Hz = 10ms timer
  timerAlarmWrite(timer, 10000, true);
  // timer enable
  timerAlarmEnable(timer);

  delay(1000);

  if (BNO055Begin) bno.setExtCrystalUse(true);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    hogehoge.ReceiveByte(Serial.read());
  }

  if (interrupt) {
    if (BNO055Begin) {
      imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
      // *(encoderModule.pulse_map[0]) = 1111;
      // *(encoderModule.pulse_map[1]) = 2222;
      // *(encoderModule.pulse_map[2]) = 3333;
      // *(encoderModule.pulse_map[3]) = 4444;
      encoderModule.position_x = euler.x();
      encoderModule.position_y = euler.y();
      encoderModule.yaw = euler.z();
    }

    sensorModule.switch_state.bits.switch_1 = digitalRead(gpio_pin);
    sensorModule.adc_1 = analogRead(adc_pin);

    interrupt = false;
  }
}
