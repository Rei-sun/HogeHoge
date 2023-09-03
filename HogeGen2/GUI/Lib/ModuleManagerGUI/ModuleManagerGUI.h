#pragma once 

#include <IPCommunicationSub.h>
#include <EncoderModuleGUI.h>
#include <MotorModuleGUI.h>
#include <SensorModuleGUI.h>
#include <SolenoidModuleGUI.h>
#include <vector>

namespace HogeGen2 {

class ModuleManagerGUI {
    template<typename T> static std::vector<T*> MakeModule(int generate_count, IPCommunicationSub *ip) {
        std::vector<T*> v;
        for (int i = 0; i < generate_count; i++) {
            v.push_back(new T(i + 1, ip));
        }
        return v;
    }
public:
    inline static std::vector<EncoderModuleGUI*> encoderModules;
    inline static std::vector<SensorModuleGUI*> sensorModules;
    inline static std::vector<MotorModuleGUI*> motorModules;
    inline static std::vector<SolenoidModuleGUI*> solenoidModules;

    template<typename T> static void SetModule(int count, IPCommunicationSub *ip);
    static void UnsetModules();
};

}