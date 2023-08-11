#pragma once

class Sensor {
protected:
  uint8_t pin;
  int value;
  Sensor(uint8_t _pin) : pin(_pin), value(0) {}
public:
  virtual void Begin() = 0;
  virtual int Read() = 0;
};

class DigitalSensor : public Sensor {
protected:
  DigitalSensor(uint8_t _pin) : Sensor(_pin) {}
public:
  virtual void Begin() = 0;
  int Read() override {
    return digitalRead(pin);
  }
};

class AnalogSensor : public Sensor {
protected:
  AnalogSensor(uint8_t _pin) : Sensor(_pin) {}
public:
  void Begin() override {};
  int Read() override {
    return analogRead(pin);
  }
};

class Button : public DigitalSensor {
public:
  Button(uint8_t _pin) : DigitalSensor(_pin) {}
  void Begin() override {
    pinMode(pin, INPUT_PULLDOWN);
  }
};

class Switch : public DigitalSensor {
public:
  Switch(uint8_t _pin) : DigitalSensor(_pin) {}
  void Begin() override {
    pinMode(pin, INPUT_PULLDOWN);
  }
};

class Potentio : public AnalogSensor {
public:
  Potentio(uint8_t _pin) : AnalogSensor(_pin) {}
};