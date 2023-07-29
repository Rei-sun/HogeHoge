#include <ModuleManager.h>

std::vector<EncoderModule*> &ModuleManager::encoderModules = EncoderModule::modules;
std::vector<SensorModule*> &ModuleManager::sensorModules = SensorModule::modules;
std::vector<MotorControlModule*> &ModuleManager::motorControlModule = MotorControlModule::modules;
std::vector<SolenoidModule*> &ModuleManager::solenoidModule = SolenoidModule::modules;