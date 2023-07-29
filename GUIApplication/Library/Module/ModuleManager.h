#pragma once

#include <Module.h>

class ModuleManager {
    static std::vector<EncoderModule*> &encoderModules;
    static std::vector<SensorModule*> &sensorModules;
    static std::vector<MotorControlModule*> &motorControlModule;
    static std::vector<SolenoidModule*> &solenoidModule;

    template<class T> static void MakeModule(int div_id) {
        for (int i = 0; i < div_id; i++) {
            new T(i + 1);
        }
    }

public:
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
};