#include <EncoderModule.h>

using namespace HogeHoge;

EncoderModule::EncoderModule(HogeHogeSerial &_serial, uint8_t _module_num):
    BaseModule(_serial, ModuleID::EncoderModule, _module_num),
    in_pulse_1(0),
    in_pulse_2(0),
    in_pulse_3(0),
    in_pulse_4(0),
    in_position_x(0.0f),
    in_position_y(0.0f),
    in_yaw(0.0f),
    value_map{
        &in_pulse_1,
        &in_pulse_2,
        &in_pulse_3,
        &in_pulse_4
    }
{

}

bool EncoderModule::Command(uint8_t cmd, uint8_t device_id, uint8_t length, void* data) {
    serial.Send((uint8_t)module_id, cmd, module_num, device_id, length, data);
    return true;
}

void EncoderModule::Receive(uint8_t cmd, uint8_t device_id, uint8_t length, void* data) {
    
}

short EncoderModule::GetPulse(uint8_t device_id) {
    // Argument check
    if (device_id > 4 | device_id < 1) return 0;

    return *value_map[device_id - 1];
}

float EncoderModule::GetPositionX() {
    return in_position_x;
}

float EncoderModule::GetPositionY() {
    return in_position_y;
}

float EncoderModule::GetEulerYaw() {
    return in_yaw;
}