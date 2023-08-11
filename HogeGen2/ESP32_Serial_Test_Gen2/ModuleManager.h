#pragma once

#include "Module.h"

#include <vector>

namespace HogeGen2 {

class ModuleManager {
    inline static std::vector<EncoderModule*> encoderModules;
    inline static std::vector<SensorModule*> sensorModules;
    inline static std::vector<MotorModule*> motorModules;
    inline static std::vector<SolenoidModule*> solenoidModules;

    template<class T> static std::vector<T*> MakeModule(int dev_id) {
        std::vector<T*> v;
        for (int i = 0; i < dev_id; i++) {
            v.push_back(new T(i + 1));
        }
        return v;
    }
public:
    static std::vector<EncoderModule*> GetEncoderModules() { return encoderModules; }

    static std::vector<SensorModule*> GetSensorModules() { return sensorModules; }

    static std::vector<MotorModule*> GetMotorModules() { return motorModules; }

    static std::vector<SolenoidModule*> GetSolenoidModules() { return solenoidModules; }

    static void MakeEncoderModule(int dev_id) {
        auto vp = MakeModule<EncoderModule>(dev_id);    
        encoderModules.insert(encoderModules.end(), vp.begin(), vp.end()); 
    }
    
    static void MakeSensorModule(int dev_id) {
        auto vp = MakeModule<SensorModule>(dev_id);
        sensorModules.insert(sensorModules.end(), vp.begin(), vp.end());
    }
    
    static void MakeMotorModule(int dev_id) {
        auto vp = MakeModule<MotorModule>(dev_id);
        motorModules.insert(motorModules.end(), vp.begin(), vp.end());
    }
    
    static void MakeSolenoidModule(int dev_id) { 
        auto vp = MakeModule<SolenoidModule>(dev_id); 
        solenoidModules.insert(solenoidModules.end(), vp.begin(), vp.end());
    }

    template<class T> static bool IsNotValidModuleNumber(uint8_t module_num);
};

template<> bool ModuleManager::IsNotValidModuleNumber<EncoderModule>(uint8_t module_num) {
    return (encoderModules.size() < module_num);
}

template<> bool ModuleManager::IsNotValidModuleNumber<SensorModule>(uint8_t module_num) {
    return (sensorModules.size() < module_num);
}

template<> bool ModuleManager::IsNotValidModuleNumber<MotorModule>(uint8_t module_num) {
    return (motorModules.size() < module_num);
}

template<> bool ModuleManager::IsNotValidModuleNumber<SolenoidModule>(uint8_t module_num) {
    return (solenoidModules.size() < module_num);
}

}