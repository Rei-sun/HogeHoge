#pragma once
#include <HogeHogeSerial.h>
#include <BaseModule.h>
#include <CommandDefinition.h>

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

        // Duty min
        const float limit_duty_min;

        // Duty max
        const float limit_duty_max;

        // Map for out_pwm
        float* const value_map[6];

        /// @brief command transmit function
        /// @param cmd command
        /// @param device_id Device ID
        /// @param length Data length
        /// @param data Data
        /// @return OK
        bool Command(uint8_t cmd, uint8_t device_id, uint8_t length, void* data) override;
        
        /// @brief Clamping value function
        /// @param duty Duty
        /// @param min min
        /// @param max max
        /// @return Clamped value
        inline float DutyClamp(float duty, float min, float max);
    public:
    
        /// @brief Delete default constructer
        MotorControlModule() = delete;

        /// @brief Constructer
        /// @param _serial Reference of HogeHogeSerial 
        /// @param module_num Module numbber
        MotorControlModule(HogeHogeSerial &_serial, uint8_t module_num);

        /// @brief batch sending
        void SendBatch() override;

        std::pair<uint8_t, std::shared_ptr<uint8_t[]>> Serialized() override;

        /// @brief Set PWM duty function
        /// @param device_id
        /// @param duty
        void SetDuty(uint8_t device_id, float duty);

        /// @brief Get PWM duty from cache function
        /// @param device_id
        float GetDuty(uint8_t device_id);
    };
}