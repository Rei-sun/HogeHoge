#include <ModuleManagerMain.h>

using namespace HogeGen2;

template<> void ModuleManagerMain::SetModule<EncoderModuleMain>(int count) {
    auto vp = ModuleManagerMain::MakeModule<EncoderModuleMain>(count);    
    encoderModules.insert(encoderModules.end(), vp.begin(), vp.end()); 
}

template<> void ModuleManagerMain::SetModule<MotorModuleMain>(int count) {
    auto vp = MakeModule<MotorModuleMain>(count);
    motorModules.insert(motorModules.end(), vp.begin(), vp.end());
}

template<> void ModuleManagerMain::SetModule<SensorModuleMain>(int count) {
    auto vp = MakeModule<SensorModuleMain>(count);
    sensorModules.insert(sensorModules.end(), vp.begin(), vp.end());
}

template<> void ModuleManagerMain::SetModule<SolenoidModuleMain>(int count) {
    auto vp = MakeModule<SolenoidModuleMain>(count); 
    solenoidModules.insert(solenoidModules.end(), vp.begin(), vp.end());
}