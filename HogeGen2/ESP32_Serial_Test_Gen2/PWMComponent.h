#pragma once

#include <stdint.h>
#include "esp32-hal-ledc.h"
#include "esp32-hal-gpio.h"

class PWMOut {
protected:
  uint32_t freq;
  uint8_t resolution;
  uint8_t channel;
  uint8_t pin;
  PWMOut(uint32_t _freq, uint8_t _res,uint8_t _ch, uint8_t _pin) : 
    freq(_freq),
    resolution(_res),
    channel(_ch),
    pin(_pin)
  {}
  virtual void Begin() = 0;
  virtual void SetDuty(float duty) = 0;
};

class LEDPWM : public PWMOut {
public:
  LEDPWM(uint8_t _ch, uint8_t _pin) : 
    PWMOut(1000, 8, _ch, _pin)
  {}
  void Begin() override {
    pinMode(pin, OUTPUT);
    ledcSetup(channel,freq,resolution);
    ledcAttachPin(pin, channel);
    ledcWrite(channel,0);
  }
  void SetDuty(float duty) override {
    float d = duty / 100.0f * 0xFF;
    ledcWrite(channel, (uint32_t)d); 
  }
};