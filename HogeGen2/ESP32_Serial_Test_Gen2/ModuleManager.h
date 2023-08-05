#pragma once

#include "Module.h"

#include <vector>

namespace HogeGen2 {

class ModuleManager {
    inline static std::vector<EncoderModule*> encoderModules;
    inline static std::vector<SensorModule*> sensorModules;
    inline static std::vector<MotorModule*> motorModules;
    inline static std::vector<SolenoidModule*> solenoidModules;

    template<class T> static std::vector<T*> MakeModule(int div_id) {
        std::vector<T*> v;
        for (int i = 0; i < div_id; i++) {
            v.push_back(new T(i + 1));
        }
        return v;
    }
public:
    static std::vector<EncoderModule*> GetEncoderModules() { return encoderModules; }

    static std::vector<SensorModule*> GetSensorModules() { return sensorModules; }

    static std::vector<MotorModule*> GetMotorModules() { return motorModules; }

    static std::vector<SolenoidModule*> GetSolenoidModules() { return solenoidModules; }

    static void MakeEncoderModule(int div_id) {
        auto vp = MakeModule<EncoderModule>(div_id);    
        encoderModules.insert(encoderModules.end(), vp.begin(), vp.end()); 
    }
    
    static void MakeSensorModule(int div_id) {
        auto vp = MakeModule<SensorModule>(div_id);
        sensorModules.insert(sensorModules.end(), vp.begin(), vp.end());
    }
    
    static void MakeMotorModule(int div_id) {
        auto vp = MakeModule<MotorModule>(div_id);
        motorModules.insert(motorModules.end(), vp.begin(), vp.end());
    }
    
    static void MakeSolenoidModule(int div_id) { 
        auto vp = MakeModule<SolenoidModule>(div_id); 
        solenoidModules.insert(solenoidModules.end(), vp.begin(), vp.end());
    }
};

}