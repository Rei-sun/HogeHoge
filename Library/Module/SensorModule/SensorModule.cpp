#include <SensorModule.h>

using namespace HogeHoge;

SensorModule::SensorModule(HogeHogeSerial &_serial, uint8_t _module_num):
    BaseModule(_serial, ModuleID::SensorModule, _module_num),
    in_swich_state{0},
    in_analog_1(0),
    in_analog_2(0),
    in_analog_3(0),
    in_analog_4(0),
    in_analog_5(0),
    in_analog_6(0),
    value_map{
        &in_analog_1,
        &in_analog_2,
        &in_analog_3,
        &in_analog_4,
        &in_analog_5,
        &in_analog_6
    }
{

}

bool SensorModule::Command(uint8_t cmd, uint8_t device_id, uint8_t length, void* data) {
    serial.Send((uint8_t)module_id, cmd, module_num, device_id, length, data);
    return true;
}

void SensorModule::Receive(uint8_t cmd, uint8_t device_id, uint8_t length, void* data) {
    
}

bool SensorModule::GetSwitch(uint8_t device_id) {
    // Argument check
    if (device_id > 7 || device_id < 1) return false;

    return in_swich_state.all >> (device_id - 1) & 1;
}

short SensorModule::GetAnalog(uint8_t device_id) {
    // Argument check
    if (device_id > 6 || device_id < 1) return false;

    return *value_map[device_id - 1];
}