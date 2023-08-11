#pragma once

class Solenoid {
  bool state;
  uint8_t pin;

public:
  Solenoid(uint8_t _pin):
    state(false),
    pin(_pin)
  {

  }

  void Begin() {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, 0);
  }

  void SetState(bool s) {
    state = s;
    digitalWrite(pin, state);
  }
};