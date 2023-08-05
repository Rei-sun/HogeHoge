#include <stdint.h>
#include "esp32-hal-ledc.h"
#include "esp32-hal-gpio.h"
#pragma once

class Motor {
  uint8_t freq;
  uint8_t resolution;
  uint8_t channel;
  uint8_t pin;

public:
  Motor(uint8_t _ch, uint8_t _pin) : 
    freq(1000),
    resolution(8),
    channel(_ch),
    pin(_pin)
  {

  }

  void Begin() {
    pinMode(pin, OUTPUT);
    ledcSetup(channel,freq,resolution);
    ledcAttachPin(pin, channel);
    ledcWrite(channel,0);
  }

  void SetDuty(float duty) {
    float d = duty / 100.0f * 0xFF;
    ledcWrite(channel, (uint32_t)d); 
  }
};