#pragma once

#include <ModuleDefinition.h>

#include <stdint.h>
#include <string.h>
#include <string>
#include <tuple>
#include <vector>

class BaseModule {
protected:
    std::string name;
    uint8_t module_id;
    uint8_t module_num;
    BaseModule(uint8_t id, uint8_t num) : module_id(id), module_num(num){}
    template<typename... Args> void SetArrays(Args... args){
        for(auto t : std::initializer_list<std::tuple<void*, void*, int>>{args...}){
            memcpy(std::get<0>(t), std::get<1>(t), std::get<2>(t));
        }
    }
    virtual void Deserialize(uint8_t*, int) = 0;
public:
    std::string GetName() { return name; }
};

class EncoderModule : public BaseModule {
    short pulse_array[4];
    float pose_array[5];
public:
    static std::vector<EncoderModule*> modules;
    EncoderModule(uint8_t num) : BaseModule((uint8_t)ModuleID::EncoderModule, num), pulse_array{0, 0, 0, 0}, pose_array{0, 0, 0, 0, 0} {
        name = "Hoge EncoderModule " + std::to_string(num);
        modules.push_back(this);
    }
    ~EncoderModule() {
        for (auto it = modules.begin(); it != modules.end(); it++) {
            if (*it == this) {
                modules.erase(it);
                break;
            }
        }
        
    }
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
    static std::vector<SensorModule*> modules;
    SensorModule(uint8_t num) : BaseModule((uint8_t)ModuleID::SensorModule, num), digital_array{0}, analog_array{0, 0, 0, 0, 0, 0} {
        name = "Hoge SensorModule " + std::to_string(num);
        modules.push_back(this);
    }
    ~SensorModule() {
        for (auto it = modules.begin(); it != modules.end(); it++) {
            if (*it == this) {
                modules.erase(it);
                break;
            }
        }
    }
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
    float duty_array[6];
public:
    static std::vector<MotorControlModule*> modules;
    MotorControlModule(uint8_t num) : BaseModule((uint8_t)ModuleID::MotorControlModule, num), duty_array{0, 0, 0, 0, 0, 0} {
        name = "Hoge MotorControlModule " + std::to_string(num);
        modules.push_back(this);
    }
    ~MotorControlModule() {
        for (auto it = modules.begin(); it != modules.end(); it++) {
            if (*it == this) {
                modules.erase(it);
                break;
            }
        }    
    }
    void Deserialize(uint8_t* data, int size) {
        float *fa = (float*)&data[2];
        SetArrays(
            std::tuple<void*, void*, int>{duty_array, &data[2], sizeof(float) * 6}
        );
    }
    float GetDuty(int dev_id) { return duty_array[dev_id - 1]; }
};

class SolenoidModule : public BaseModule {
    uint8_t state_array[1];
public:
    static std::vector<SolenoidModule*> modules;
    SolenoidModule(uint8_t num) : BaseModule((uint8_t)ModuleID::SolenoidModule, num),state_array{0} {
        name = "Hoge SolenoidModule " + std::to_string(num);
        modules.push_back(this);
    }
    ~SolenoidModule() {
        for (auto it = modules.begin(); it != modules.end(); it++) {
            if (*it == this) {
                modules.erase(it);
                break;
            }
        }    
    }
    void Deserialize(uint8_t* data, int size) {
        uint8_t *ba = (uint8_t*)&data[2];
        SetArrays(
            std::tuple<void*, void*, int>{state_array, &data[2], sizeof(uint8_t) * 1}
        );
    }
    bool GetState(int dev_id) { return state_array[0] >> (dev_id - 1) & 1; }
};