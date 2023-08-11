#pragma once

#include "ModuleDefinition.h"
#include "BaseModule.h"

class EncoderMod : public BaseModule {
public:
  short pulse_1;
  short pulse_2;
  short pulse_3;
  short pulse_4;
  float position_x;
  float position_y;
  float roll;
  float pitch;
  float yaw;

  short* const pulse_map[4];

  EncoderMod(uint8_t _module_num) :
    BaseModule(ModuleID::EncoderModule, _module_num),
    pulse_1(0),
    pulse_2(0),
    pulse_3(0),
    pulse_4(0),
    position_x(0.f),
    position_y(0.f),
    roll(0.f),
    pitch(0.f),
    yaw(0.f),
    pulse_map{
      &pulse_1,
      &pulse_2,
      &pulse_3,
      &pulse_4
    }
  {

  }

  short GetPulse(uint8_t device_id) {
    if (device_id > 4 | device_id < 1) return 0;
    return *pulse_map[device_id - 1];
  }

  float GetPositionX() {
    return position_x;
  }

  float GetPositionY() {
    return position_y;
  }

  float GetEulerRoll() {
    return roll;
  }

  float GetEulerPitch() {
    return pitch;
  }

  float GetEulerYaw() {
      return yaw;
  }
};