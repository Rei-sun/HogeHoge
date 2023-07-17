#include <SensorModule.h>

using namespace HogeHoge;

SensorModule::SensorModule(HogeHogeSerial &_serial, uint8_t _module_num):
    BaseModule(_serial, ModuleID::SensorModule, _module_num)
{

}

bool SensorModule::Command(uint8_t cmd, uint8_t device_id, uint8_t length, void* data) {
    serial.Send((uint8_t)module_id, cmd, module_num, device_id, length, data);
    return true;
}

void SensorModule::Receive(uint8_t cmd, uint8_t device_id, uint8_t length, void* data) {
    
}