#pragma once

#include "Utility.h"

#include <stdint.h>

namespace HogeGen2{

enum class ModuleID {
  UndefinedModule,
  MotorModule,
  EncoderModule,
  SensorModule,
  SolenoidModule
};

class BaseModule {
protected:
    uint8_t module_id;
    uint8_t module_num;
};

class EncoderModule : public BaseModule {
protected:
    short pulse_array[4];
    float pose_array[5];
public:
    short GetPulse(uint8_t dev_id) { return pulse_array[dev_id - 1]; }
    float GetPose(uint8_t dev_id) { return pose_array[dev_id - 1]; }
};

class SensorModule : public BaseModule {
protected:
    typedef struct {
        unsigned digital_1: 1;
        unsigned digital_2: 1;
        unsigned digital_3: 1;
        unsigned digital_4: 1;
        unsigned digital_5: 1;
        unsigned digital_6: 1;
    }  in_digital_struct;

    typedef union {
        uint8_t all;
        in_digital_struct bits;
    } in_digital;

    in_digital digital_array[1];
    short analog_array[6];
public:
    bool GetDigital(uint8_t dev_id) { return digital_array[0].all >> (dev_id - 1) & 1; }
    short GetAnalog(uint8_t dev_id) { return analog_array[dev_id - 1]; }
};

class MotorModule : public BaseModule {
protected:
    float duty_array[6];
    const float limit_duty_min;
    const float limit_duty_max;
public:
    void SetDuty(uint8_t dev_id, float duty) { duty_array[dev_id - 1] = Clamp(duty, limit_duty_min, limit_duty_max); }
    float GetDuty(uint8_t dev_id) { return duty_array[dev_id - 1]; }
};

class SolenoidModule : public BaseModule {
protected:
    typedef struct {
        unsigned state_1: 1;
        unsigned state_2: 1;
        unsigned state_3: 1;
        unsigned state_4: 1;
        unsigned state_5: 1;
        unsigned state_6: 1;
        unsigned state_7: 1;
        unsigned state_8: 1;
    }  in_state_struct;
    
    typedef union {
        uint8_t all;
        in_state_struct bits;
    } in_state;

    in_state state_array[1];
public:
    void SetState(uint8_t device_id, bool state) { state_array[0].all ^= (state << (device_id - 1)); }
    bool GetState(uint8_t dev_id) { return state_array[0].all >> (dev_id - 1) & 1; }
};

}