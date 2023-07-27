#pragma once

#include <ModuleDefinition.h>

#include <stdint.h>
#include <string.h>
#include <tuple>

class BaseModule {
protected:
    uint8_t module_id;
    uint8_t module_num;
    BaseModule(uint8_t id, uint8_t num) : module_id(id), module_num(num){}
    virtual void Deserialize(uint8_t*, int) = 0;
    template<typename... Args> void SetArrays(Args... args){
        for(auto t : std::initializer_list<std::tuple<void*, void*, int>>{args...}){
            memcpy(std::get<0>(t), std::get<1>(t), std::get<2>(t));
        }
    }
};

class EncoderModule : public BaseModule {
    short pulse_array[4];
    float pose_array[5];
public:
    EncoderModule(uint8_t num) : BaseModule((uint8_t)ModuleID::EncoderModule, num), pulse_array{0, 0, 0, 0}, pose_array{0, 0, 0, 0, 0} {}
    void Deserialize(uint8_t* data, int size) {
        SetArrays(
            std::tuple<void*, void*, int>{pulse_array, &data[2], sizeof(short) * 4},
            std::tuple<void*, void*, int>{pose_array, &data[10], sizeof(float) * 5}
        );
    }
    short GetPulse(int dev_id) { return pulse_array[dev_id - 1]; }
    float GetPose(int dev_id) { return pose_array[dev_id - 1]; }
};

class SensorModule : public BaseModule {
    uint8_t digital_array[1];
    short analog_array[6];
public:
    SensorModule(uint8_t num) : BaseModule((uint8_t)ModuleID::SensorModule, num), digital_array{0}, analog_array{0, 0, 0, 0, 0, 0} {}
    void Deserialize(uint8_t* data, int size) {
        uint8_t *ba = (uint8_t*)&data[2];
        short *sa = (short*)&data[3];
        SetArrays(
            std::tuple<void*, void*, int>{digital_array, &data[2], sizeof(uint8_t) * 1},
            std::tuple<void*, void*, int>{analog_array, &data[3], sizeof(short) * 6}
        );
    }
    bool GetDigital(int dev_id) { return digital_array[0] >> (dev_id - 1) & 1; }
    short GetAnalog(int dev_id) { return analog_array[dev_id - 1]; }
};

class MotorControlModule : public BaseModule {
    short duty_array[6];
public:
    MotorControlModule(uint8_t num) : BaseModule((uint8_t)ModuleID::MotorControlModule, num), duty_array{0, 0, 0, 0, 0, 0} {}
    void Deserialize(uint8_t* data, int size) {
        float *fa = (float*)&data[2];
        SetArrays(
            std::tuple<void*, void*, int>{duty_array, &data[2], sizeof(float) * 6}
        );
    }
    short GetDuty(int dev_id) { return duty_array[dev_id - 1]; }
};

class SolenoidModule : public BaseModule {
    uint8_t state_array[1];
public:
    SolenoidModule(uint8_t num) : BaseModule((uint8_t)ModuleID::SolenoidModule, num),state_array{0} {}
    void Deserialize(uint8_t* data, int size) {
        uint8_t *ba = (uint8_t*)&data[2];
        SetArrays(
            std::tuple<void*, void*, int>{state_array, &data[2], sizeof(uint8_t) * 1}
        );
    }
    bool GetState(int dev_id) { return state_array[0] >> (dev_id - 1) & 1; }
};