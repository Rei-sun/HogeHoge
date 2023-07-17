#pragma once
#include <HogeHogeSerial.h>
#include <BaseModule.h>

namespace HogeHoge {
    
    /// @brief Motor control module class
    class MotorControlModule : public BaseModule {
        // device output
        float out_pwm_1;    // device_id = 1, output
        float out_pwm_2;    // device_id = 2, output
        float out_pwm_3;    // device_id = 3, output
        float out_pwm_4;    // device_id = 4, output
        float out_pwm_5;    // device_id = 5, output
        float out_pwm_6;    // device_id = 6, output

        // device input
        // none

    public:
        /// @brief command transmit function
        /// @param cmd command
        /// @param device_id Device ID
        /// @param length Data length
        /// @param data Data
        /// @return OK
        bool Command(uint8_t cmd, uint8_t device_id, uint8_t length, void* data) override;

        /// @brief Delete default constructer
        MotorControlModule() = delete;

        /// @brief Constructer
        /// @param _serial Reference of HogeHogeSerial 
        /// @param module_num Module numbber
        MotorControlModule(HogeHogeSerial &_serial, uint8_t module_num);
    };
}