#pragma once

class Output {
protected:
  uint8_t pin;
  int value;
  Output(uint8_t _pin) : pin(_pin), value(0) {}
public:
  virtual void Begin() = 0;
  virtual void Write(bool s) = 0;
};

class LEDOut : public Output {
public:
  LEDOut(uint8_t _pin) : Output(_pin) {}
  void Begin() override {
    pinMode(pin, OUTPUT);
  }
  void Write(bool s) override {
    value = s;
    digitalWrite(pin, value);
  }
};

class Solenoid : public Output {
public:
  Solenoid(uint8_t _pin) : Output(_pin) {}
  void Begin() override {
    pinMode(pin, OUTPUT);
  }
  void Write(bool s) override {
    value = s;
    digitalWrite(pin, value);
  }
};