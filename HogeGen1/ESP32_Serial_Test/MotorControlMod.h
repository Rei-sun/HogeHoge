#pragma once

#include "ModuleDefinition.h"
#include "BaseModule.h"

#include "Motor.h"

class MotorControlMod : public BaseModule {
public:
  float duty_1;
  float duty_2;
  float duty_3;
  float duty_4;
  float duty_5;
  float duty_6;

  float* const duty_map[6];

  Motor motor_1;
  Motor motor_2;
  Motor motor_3;
  Motor motor_4;

  MotorControlMod(uint8_t _module_num, uint8_t ch_1, uint8_t pin_1, uint8_t ch_2, uint8_t pin_2, uint8_t ch_3, uint8_t pin_3, uint8_t ch_4, uint8_t pin_4) :
    BaseModule(ModuleID::MotorControlModule, _module_num), 
    duty_1(0.f),
    duty_2(0.f),
    duty_3(0.f),
    duty_4(0.f),
    duty_5(0.f),
    duty_6(0.f),
    duty_map{
      &duty_1,
      &duty_2,
      &duty_3,
      &duty_4,
      &duty_5,
      &duty_6
    },
    motor_1(ch_1, pin_1),
    motor_2(ch_2, pin_2),
    motor_3(ch_3, pin_3),
    motor_4(ch_4, pin_4)
  {

  }

  void Begin() {
    motor_1.Begin();
    motor_2.Begin();
    motor_3.Begin();
    motor_4.Begin();
  }
  
  void UpdateAll() {
    motor_1.SetDuty(duty_1);
    motor_2.SetDuty(duty_2);
    motor_3.SetDuty(duty_3);
    motor_4.SetDuty(duty_4);
  }
};