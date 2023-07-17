#include <MotorControlModule.h>

using namespace HogeHoge;

bool MotorControlModule::Command(uint8_t cmd, uint8_t device_id, uint8_t length, void* data) {
    serial.Send((uint8_t)module_id, cmd, module_num, device_id, length, data);
    return true;
}

float MotorControlModule::DutyClamp(float duty, float low, float high) {
    return std::min(std::max(duty, low), high);
}

MotorControlModule::MotorControlModule(HogeHogeSerial &_serial, uint8_t _module_num):
    BaseModule(_serial, ModuleID::MotorControlModule, _module_num),
    out_pwm_1(0.f),
    out_pwm_2(0.f),
    out_pwm_3(0.f),
    out_pwm_4(0.f),
    out_pwm_5(0.f),
    out_pwm_6(0.f),
    limit_duty_min(-100.0f),
    limit_duty_max(100.0f),
    value_map {
        &out_pwm_1,
        &out_pwm_2,
        &out_pwm_3,
        &out_pwm_4,
        &out_pwm_5,
        &out_pwm_6
    }
{

}

void MotorControlModule::SendBatch() {
    Command((uint8_t)CMD_MotorControlModule::SetAllDuty, 0, sizeof(float) * 6, (void *)value_map[0]);
}

void MotorControlModule::SetDuty(uint8_t device_id, float duty) {
    // Argument check
    if (device_id > 6 || device_id < 1) return;

    *value_map[device_id - 1] = DutyClamp(duty, limit_duty_min, limit_duty_max);
}

float MotorControlModule::GetDuty(uint8_t device_id) {
    // Argument check
    if (device_id > 6 || device_id < 1) return 9999.f;

    return *value_map[device_id - 1];
}