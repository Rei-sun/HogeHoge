#pragma once

#include <Module.h>

class ModuleManager {
    static std::vector<EncoderModule*> &encoderModules;
    static std::vector<SensorModule*> &sensorModules;
    static std::vector<MotorControlModule*> &motorControlModules;
    static std::vector<SolenoidModule*> &solenoidModules;

    template<class T> static void MakeModule(int div_id) {
        for (int i = 0; i < div_id; i++) {
            new T(i + 1);
        }
    }

public:
    static std::vector<EncoderModule*> GetEncoderModules() {
        return encoderModules;
    }

    static std::vector<SensorModule*> GetSensorModules() {
        return sensorModules;
    }

    static std::vector<MotorControlModule*> GetMotorControlModules() {
        return motorControlModules;
    }

    static std::vector<SolenoidModule*> GetSolenoidModules() {
        return solenoidModules;
    }

    static void MakeEncoderModule(int div_id) {
        MakeModule<EncoderModule>(div_id);
    }
    static void MakeSensorModule(int div_id) {
        MakeModule<SensorModule>(div_id);
    }
    static void MakeMotorControlModule(int div_id) {
        MakeModule<MotorControlModule>(div_id);
    }
    static void MakeSolenoidModule(int div_id) {
        MakeModule<SolenoidModule>(div_id);
    }
    static void ReceiveData(uint8_t module_id, uint8_t module_num, uint8_t *data, int size) {
        if (module_id  == (uint8_t)ModuleID::EncoderModule) {
            if(encoderModules.size() < module_num || module_num == 0) return;
            encoderModules[module_num - 1]->Deserialize((uint8_t*)data, size);
        } else if (module_id  == (uint8_t)ModuleID::SensorModule) {
            if(sensorModules.size() < module_num || module_num == 0) return;
            sensorModules[module_num - 1]->Deserialize((uint8_t*)data, size);
        } else if (module_id  == (uint8_t)ModuleID::MotorControlModule) {
            if(motorControlModules.size() < module_num || module_num == 0) return;
            motorControlModules[module_num - 1]->Deserialize((uint8_t*)data, size);
        } else if (module_id  == (uint8_t)ModuleID::SolenoidModule) {
            if(solenoidModules.size() < module_num || module_num == 0) return;
            solenoidModules[module_num - 1]->Deserialize((uint8_t*)data, size);
        }
    }
};