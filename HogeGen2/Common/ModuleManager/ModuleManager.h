#pragma once

#include "Module.h"

#include <vector>

namespace HogeGen2{
class ModuleManager {
    static std::vector<EncoderModule*> encoderModules;
    static std::vector<SensorModule*> sensorModules;
    static std::vector<MotorModule*> motorModules;
    static std::vector<SolenoidModule*> solenoidModules;

    template<class T> static void MakeModule(int div_id) { for (int i = 0; i < div_id; i++) new T(i + 1); }  
public:
    std::vector<EncoderModule*> GetEncoderModules() { return encoderModules; }

    std::vector<SensorModule*> GetSensorModules() { return sensorModules; }

    std::vector<MotorModule*> GetMotorControlModules() { return motorModules; }

    std::vector<SolenoidModule*> GetSolenoidModules() { return solenoidModules; }

    static void MakeEncoderModule(int div_id) { MakeModule<EncoderModule>(div_id); }
    
    static void MakeSensorModule(int div_id) { MakeModule<SensorModule>(div_id); }
    
    static void MakeMotorControlModule(int div_id) { MakeModule<MotorModule>(div_id); }
    
    static void MakeSolenoidModule(int div_id) { MakeModule<SolenoidModule>(div_id); }

    static void ReceiveData(uint8_t module_id, uint8_t module_num, uint8_t *data, int size) {}
};
}