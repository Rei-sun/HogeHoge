#pragma once

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

class nDOF {
public:
  virtual void Begin() = 0;
  virtual void Update() = 0;
  virtual float GetRoll() = 0;
  virtual float GetPitch() = 0;
  virtual float GetYaw() = 0;
};

class BNO055 : public nDOF {
  Adafruit_BNO055 &bno;
  bool BNO055Begin;
  float euler[3];
public:
  BNO055(Adafruit_BNO055 &_bno) : 
    bno(_bno),
    BNO055Begin(false),
    euler{
      0.f,
      0.f,
      0.f
    }
  {

  }
  void Begin() override {
    BNO055Begin = bno.begin();
    delay(1000);
    if (BNO055Begin) bno.setExtCrystalUse(true);
  }

  void Update() override {
    if (BNO055Begin) {
      imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
      euler[0] = euler.x();
      euler[1] = euler.y();
      euler[2] = euler.z();
    }
  }

  float GetRoll() override { return euler[1]; }
  float GetPitch() override { return euler[2]; }
  float GetYaw() override { return euler[0]; }
};