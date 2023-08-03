#include "HogeHogeSerial.h"
#include "EncoderMod.h"
#include "SensorMod.h"
#include "MotorControlMod.h"
#include "SolenoidMod.h"

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define BNO055_SAMPLERATE_DELAY_MS (100)

hw_timer_t * timer = NULL;

int input_pin_1 = 13;
int input_pin_2 = 12;
int input_pin_3 = 14;
int input_pin_4 = 27;
int input_pin_5 = 15;
int adc_pin_1 = 34;
int output_pin_1 = 18;
int output_pin_2 = 19;
int output_pin_3 = 23;
int pwm_pin_1 = 26;
int pwm_ch_1 = 0;
int pwm_pin_2 = 25;
int pwm_ch_2 = 1;
int pwm_pin_3 = 33;
int pwm_ch_3 = 2;
int pwm_pin_4 = 32;
int pwm_ch_4 = 3;

EncoderMod encoderModule(1);
SensorMod sensorModule(1);
MotorControlMod motorControlModule(1, pwm_ch_1, pwm_pin_1, pwm_ch_2, pwm_pin_2, pwm_ch_3, pwm_pin_3, pwm_ch_4, pwm_pin_4);
SolenoidMod solenoidModule(1, output_pin_1, output_pin_2, output_pin_3);

Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28);
HogeHogeSerial hogehoge(encoderModule, sensorModule, motorControlModule, solenoidModule);

bool BNO055Begin = false;
volatile bool interrupt = false;

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
  pinMode(input_pin_1, INPUT_PULLDOWN);
  pinMode(input_pin_2, INPUT_PULLDOWN);
  pinMode(input_pin_3, INPUT_PULLDOWN);
  pinMode(input_pin_4, INPUT_PULLDOWN);
  pinMode(input_pin_5, INPUT_PULLDOWN);
  
  solenoidModule.Begin();
  
  //-----------------------------------
  // PWM 設定
  //-----------------------------------
  motorControlModule.Begin();

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
      encoderModule.yaw = euler.x();
      encoderModule.pitch = euler.y();
      encoderModule.roll = euler.z();
    }

    sensorModule.switch_state.bits.switch_1 = digitalRead(input_pin_1);
    sensorModule.switch_state.bits.switch_2 = digitalRead(input_pin_2);
    sensorModule.switch_state.bits.switch_3 = digitalRead(input_pin_3);
    sensorModule.switch_state.bits.switch_4 = digitalRead(input_pin_4);
    sensorModule.switch_state.bits.switch_5 = digitalRead(input_pin_5);
    sensorModule.adc_1 = analogRead(adc_pin_1);

    motorControlModule.UpdateAll();
    solenoidModule.UpdateAll();

    //Serial.printf("%d, %d, %d, %d\n", sensorModule.switch_state.bits.switch_1, sensorModule.switch_state.bits.switch_2, sensorModule.switch_state.bits.switch_3, sensorModule.switch_state.bits.switch_4);

    interrupt = false;
  }
}
