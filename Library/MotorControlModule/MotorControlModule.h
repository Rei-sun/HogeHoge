#pragma once
#include <HogeHogeSerial.h>
#include <BaseModule.h>

namespace HogeHoge {
    class MotorControlModule : public BaseModule {
        // 各デバイスの出力
        float pwm_0;
        float pwm_1;
        float pwm_2;
        float pwm_3;
        float pwm_4;
        float pwm_5;

    public:
        MotorControlModule() = delete;
        MotorControlModule(HogeHogeSerial &_serial, uint8_t module_num);
        bool Command(uint8_t cmd, uint8_t device_id, uint8_t length, void* data) override;
    };
}