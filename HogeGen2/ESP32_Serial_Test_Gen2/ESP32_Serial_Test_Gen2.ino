#include "HogeHogeSerial.h"
#include "nDOF.h"
#include "PWMComponent.h"
#include "OutputComponent.h"
#include "InputComponent.h"

hw_timer_t * timer = NULL;

int input_pin_1 = 13;   // button
int input_pin_2 = 12;   // button
int input_pin_3 = 14;   // button
int input_pin_4 = 27;   // button
int input_pin_5 = 15;   // switch
int adc_pin_1 = 34;     // potentio
int output_pin_1 = 18;  // led
int output_pin_2 = 19;  // led
int output_pin_3 = 23;  // led
int pwm_pin_1 = 26;     // led
int pwm_ch_1 = 0;
int pwm_pin_2 = 25;     // led
int pwm_ch_2 = 1;
int pwm_pin_3 = 33;     // led
int pwm_ch_3 = 2;
int pwm_pin_4 = 32;     // led
int pwm_ch_4 = 3;

volatile bool interrupt = false;

Button button_1(input_pin_1);
Button button_2(input_pin_2);
Button button_3(input_pin_3);
Button button_4(input_pin_4);
Switch switch_1(input_pin_5);

Potentio potentio_1(adc_pin_1);

LEDPWM led_pwm_1(pwm_ch_1, pwm_pin_1);
LEDPWM led_pwm_2(pwm_ch_2, pwm_pin_2);
LEDPWM led_pwm_3(pwm_ch_3, pwm_pin_3);
LEDPWM led_pwm_4(pwm_ch_4, pwm_pin_4);

LEDOut led_out_1(output_pin_1);
LEDOut led_out_2(output_pin_2);
LEDOut led_out_3(output_pin_3);

Adafruit_BNO055 bno;
bool BNO055Begin = false;

HogeHogeSerial hogehoge;


void IRAM_ATTR onTimer() {
  interrupt = true;
}

void StartIndicate() {
  led_pwm_1.SetDuty(50.0f);
  led_pwm_2.SetDuty(60.0f);
  led_pwm_3.SetDuty(80.0f);
  led_pwm_4.SetDuty(100.0f);

  led_out_1.Write(true);
  led_out_2.Write(true);
  led_out_3.Write(true);

  delay(1000);

  led_pwm_1.SetDuty(0.0f);
  led_pwm_2.SetDuty(0.0f);
  led_pwm_3.SetDuty(0.0f);
  led_pwm_4.SetDuty(0.0f);

  led_out_1.Write(false);
  led_out_2.Write(false);
  led_out_3.Write(false);
}

void setup() {
  ModuleManager::MakeEncoderModule(1);
  ModuleManager::MakeSensorModule(1);
  ModuleManager::MakeMotorModule(1);
  ModuleManager::MakeSolenoidModule(1);
  //-----------------------------------
  // シリアル 設定
  //-----------------------------------
  Serial.begin(2000000);
  while (!Serial) delay(10);

  //-----------------------------------
  // ペリフェラル 設定
  //-----------------------------------
  button_1.Begin();
  button_2.Begin();
  button_3.Begin();
  button_4.Begin();
  switch_1.Begin();

  potentio_1.Begin();

  led_pwm_1.Begin();
  led_pwm_2.Begin();
  led_pwm_3.Begin();
  led_pwm_4.Begin();

  led_out_1.Begin();
  led_out_2.Begin();
  led_out_3.Begin();

  StartIndicate();
  
  BNO055Begin = bno.begin();
  delay(1000);
  if (BNO055Begin) bno.setExtCrystalUse(true);

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
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    hogehoge.ReceiveByte(Serial.read());
  }

  if (interrupt) {
    ModuleManager::GetSensorModules()[0]->SetDigital(1, button_1.Read());
    ModuleManager::GetSensorModules()[0]->SetDigital(2, button_2.Read());
    ModuleManager::GetSensorModules()[0]->SetDigital(3, button_3.Read());
    ModuleManager::GetSensorModules()[0]->SetDigital(4, button_4.Read());
    ModuleManager::GetSensorModules()[0]->SetDigital(5, switch_1.Read());

    ModuleManager::GetSensorModules()[0]->SetAnalog(1, potentio_1.Read());

    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    if (BNO055Begin) {
      euler[0] = euler.x();
      euler[1] = euler.y();
      euler[2] = euler.z();
    }
    ModuleManager::GetEncoderModules()[0]->SetPose(1, 0.f);
    ModuleManager::GetEncoderModules()[0]->SetPose(2, 0.f);
    ModuleManager::GetEncoderModules()[0]->SetPose(3, euler[1]);
    ModuleManager::GetEncoderModules()[0]->SetPose(4, euler[2]);
    ModuleManager::GetEncoderModules()[0]->SetPose(5, euler[0]);

    led_pwm_1.SetDuty(ModuleManager::GetMotorModules()[0]->GetDuty(1));
    led_pwm_2.SetDuty(ModuleManager::GetMotorModules()[0]->GetDuty(2));
    led_pwm_3.SetDuty(ModuleManager::GetMotorModules()[0]->GetDuty(3));
    led_pwm_4.SetDuty(ModuleManager::GetMotorModules()[0]->GetDuty(4));

    led_out_1.Write(ModuleManager::GetSolenoidModules()[0]->GetState(1));
    led_out_2.Write(ModuleManager::GetSolenoidModules()[0]->GetState(2));
    led_out_3.Write(ModuleManager::GetSolenoidModules()[0]->GetState(3));
    
    interrupt = false; 
  }
}
