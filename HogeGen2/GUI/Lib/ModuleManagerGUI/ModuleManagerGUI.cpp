#include <ModuleManagerGUI.h>

using namespace HogeGen2;

template<> void ModuleManagerGUI::SetModule<EncoderModuleGUI>(int count) {
    auto vp = ModuleManagerGUI::MakeModule<EncoderModuleGUI>(count);    
    encoderModules.insert(encoderModules.end(), vp.begin(), vp.end()); 
}

template<> void ModuleManagerGUI::SetModule<MotorModuleGUI>(int count) {
    auto vp = MakeModule<MotorModuleGUI>(count);
    motorModules.insert(motorModules.end(), vp.begin(), vp.end());
}

template<> void ModuleManagerGUI::SetModule<SensorModuleGUI>(int count) {
    auto vp = MakeModule<SensorModuleGUI>(count);
    sensorModules.insert(sensorModules.end(), vp.begin(), vp.end());
}

template<> void ModuleManagerGUI::SetModule<SolenoidModuleGUI>(int count) {
    auto vp = MakeModule<SolenoidModuleGUI>(count); 
    solenoidModules.insert(solenoidModules.end(), vp.begin(), vp.end());
}