#pragma once
#include <HogeHogeSerial.h>
#include <BaseModule.h>
#include <CommandDefinition.h>

namespace HogeHoge {
    class EncoderModule : public BaseModule {
        // device output
        // none

        // device input
        short in_pulse_1;       // device_id = 1, input
        short in_pulse_2;       // device_id = 2, input
        short in_pulse_3;       // device_id = 3, input
        short in_pulse_4;       // device_id = 4, input
        float in_position_x;    // position axis x [mm] (module_num = 1 only)
        float in_position_y;    // position axis y [mm] (module_num = 1 only)
        float in_roll;          // roll [deg] (module_num = 1 only)
        float in_pitch;         // pitch [deg] (module_num = 1 only)
        float in_yaw;           // yaw [deg] (module_num = 1 only)

        short* const value_map[4];

        /// @brief command transmit function
        /// @param cmd command
        /// @param device_id Device ID
        /// @param length Data length
        /// @param data Data
        /// @return OK
        bool Command(uint8_t cmd, uint8_t device_id, uint8_t length, void* data) override;

    public:
        /// @brief Delete default constructer
        EncoderModule() = delete;

        /// @brief Constructer
        /// @param _serial Reference of HogeHogeSerial 
        /// @param module_num Module numbber
        EncoderModule(HogeHogeSerial &_serial, uint8_t module_num);

        /// @brief Received data processing function
        /// @param cmd Command
        /// @param device_id Device ID
        /// @param length Size of received data size
        /// @param data received data
        void Receive(uint8_t cmd, uint8_t device_id, uint8_t length, void* data) override;

        std::pair<uint8_t, std::shared_ptr<uint8_t[]>> Serialized() override;

        void SendGetLocalization();
        void SendGetAllPulse();

        short GetPulse(uint8_t device_id);
        float GetPositionX();
        float GetPositionY();
        float GetEulerRoll();
        float GetEulerPitch();
        float GetEulerYaw();
    };
}