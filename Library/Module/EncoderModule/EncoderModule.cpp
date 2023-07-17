#include <EncoderModule.h>

using namespace HogeHoge;

EncoderModule::EncoderModule(HogeHogeSerial &_serial, uint8_t _module_num):
    BaseModule(_serial, ModuleID::EncoderModule, _module_num)
{

}

bool EncoderModule::Command(uint8_t cmd, uint8_t device_id, uint8_t length, void* data) {
    serial.Send((uint8_t)module_id, cmd, module_num, device_id, length, data);
    return true;
}

void EncoderModule::Receive(uint8_t cmd, uint8_t device_id, uint8_t length, void* data) {
    
}