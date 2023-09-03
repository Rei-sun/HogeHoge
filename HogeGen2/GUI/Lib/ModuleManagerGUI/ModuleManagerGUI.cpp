#include <ModuleManagerGUI.h>

using namespace HogeGen2;

template<> void ModuleManagerGUI::SetModule<EncoderModuleGUI>(int count, IPCommunicationSub *ip) {
    auto vp = MakeModule<EncoderModuleGUI>(count, ip);    
    encoderModules.insert(encoderModules.end(), vp.begin(), vp.end()); 
}

template<> void ModuleManagerGUI::SetModule<MotorModuleGUI>(int count, IPCommunicationSub *ip) {
    auto vp = MakeModule<MotorModuleGUI>(count, ip);
    motorModules.insert(motorModules.end(), vp.begin(), vp.end());
}

template<> void ModuleManagerGUI::SetModule<SensorModuleGUI>(int count, IPCommunicationSub *ip) {
    auto vp = MakeModule<SensorModuleGUI>(count, ip);
    sensorModules.insert(sensorModules.end(), vp.begin(), vp.end());
}

template<> void ModuleManagerGUI::SetModule<SolenoidModuleGUI>(int count, IPCommunicationSub *ip) {
    auto vp = MakeModule<SolenoidModuleGUI>(count, ip); 
    solenoidModules.insert(solenoidModules.end(), vp.begin(), vp.end());
}

void ModuleManagerGUI::UnsetModules() {
    for (auto module : encoderModules) {
        delete module;
    }
    encoderModules.clear();
    module_composition.SetCount(ModuleID::EncoderModule, 0);

    for (auto module : sensorModules) {
        delete module;
    }
    sensorModules.clear();
    module_composition.SetCount(ModuleID::SensorModule, 0);

    for (auto module : motorModules) {
        delete module;
    }
    motorModules.clear();
    module_composition.SetCount(ModuleID::MotorModule, 0);

    for (auto module : solenoidModules) {
        delete module;
    }
    solenoidModules.clear();
    module_composition.SetCount(ModuleID::SolenoidModule, 0);
}