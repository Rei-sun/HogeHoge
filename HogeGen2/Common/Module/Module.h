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
    ModuleID module_id;
    uint8_t module_num;
    BaseModule(ModuleID id, uint8_t num) : module_id(id), module_num(num), wait_for_response(false) {}
private:
    bool wait_for_response;
public:
    void SetWaitForResponse(bool s) { wait_for_response = s; }
    bool GetWaitForResponse() { return wait_for_response; }
};

class EncoderModule : public BaseModule {
protected:
    short pulse_array[4];
    float pose_array[5];
public:
    EncoderModule(uint8_t num) : BaseModule(ModuleID::EncoderModule, num) { }
    short GetPulse(uint8_t dev_id) { return pulse_array[dev_id - 1]; }
    float GetPose(uint8_t dev_id) { return pose_array[dev_id - 1]; }
    void SetPulse(uint8_t dev_id, short value) { pulse_array[dev_id - 1] = value; }
    void SetPose(uint8_t dev_id, float value) { pose_array[dev_id - 1] = value; }
    short *GetPulseArray() { return pulse_array; }
    float *GetPoseArray() { return pose_array; }
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
    SensorModule(uint8_t num) : BaseModule(ModuleID::SensorModule, num) { }
    bool GetDigital(uint8_t dev_id) { return digital_array[0].all >> (dev_id - 1) & 1; }
    short GetAnalog(uint8_t dev_id) { return analog_array[dev_id - 1]; }
    void SetDigital(uint8_t dev_id, bool value) { if (value) digital_array[0].all |= (value << (dev_id - 1)); else digital_array[0].all &= ~(value << (dev_id - 1)); }
    void SetAnalog(uint8_t dev_id, short value) { analog_array[dev_id - 1] = value; }
    in_digital *GetDigitalArray() { return digital_array; }
    short *GetAnalogArray() { return analog_array; }
};

class MotorModule : public BaseModule {
protected:
    float duty_array[6];
    const float limit_duty_min;
    const float limit_duty_max;
public:
    MotorModule(uint8_t num) : BaseModule(ModuleID::MotorModule, num), limit_duty_min(-100.f), limit_duty_max(100.f) { }
    void SetDuty(uint8_t dev_id, float duty) { duty_array[dev_id - 1] = Clamp(duty, limit_duty_min, limit_duty_max); }
    float GetDuty(uint8_t dev_id) { return duty_array[dev_id - 1]; }
    float *GetDutyArray() { return duty_array; }
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
    SolenoidModule(uint8_t num) : BaseModule(ModuleID::SolenoidModule, num) { }
    void SetState(uint8_t device_id, bool state) { if (state) state_array[0].all |= (state << (device_id - 1)); else state_array[0].all &= ~(state << (device_id - 1)); }
    bool GetState(uint8_t dev_id) { return state_array[0].all >> (dev_id - 1) & 1; }
    void SetAllState(uint8_t state) { state_array[0].all = state; }
    in_state *GetStateArray() { return state_array; }
};

}