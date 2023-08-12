#pragma once 

#include <EncoderModuleMain.h>
#include <MotorModuleMain.h>
#include <SensorModuleMain.h>
#include <SolenoidModuleMain.h>

namespace HogeGen2 {

class ModuleManagerMain {
    template<typename T> static std::vector<T*> MakeModule(int generate_count) {
        std::vector<T*> v;
        for (int i = 0; i < generate_count; i++) {
            v.push_back(new T(i + 1));
        }
        return v;
    }
public:
    inline static std::vector<EncoderModuleMain*> encoderModules;
    inline static std::vector<SensorModuleMain*> sensorModules;
    inline static std::vector<MotorModuleMain*> motorModules;
    inline static std::vector<SolenoidModuleMain*> solenoidModules;

    template<typename T> static void SetModule(int count);
};

}