#pragma once
#include <HogeHogeSerial.h>
#include <BaseModule.h>

namespace HogeHoge {
    class SolenoidModule : public BaseModule {
        // device output
        // none

        // device input
        short in_pulse_1;       // device_id = 1, input
        short in_pulse_2;       // device_id = 2, input
        short in_pulse_3;       // device_id = 3, input
        short in_pulse_4;       // device_id = 4, input
        float in_position_x;    // position axis x [mm] (module_num = 1 only)
        float in_position_y;    // position axis y [mm] (module_num = 1 only)
        float in_yaw;           // yaw [rad] (module_num = 1 only)

        bool Command(uint8_t cmd, uint8_t device_id, uint8_t length, void* data) override;
    public:
        SolenoidModule() = delete;
        SolenoidModule(HogeHogeSerial &_serial, uint8_t module_num);
        void Receive(uint8_t cmd, uint8_t device_id, uint8_t length, void* data) override;
    };
}