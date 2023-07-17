#pragma once
#include <HogeHogeSerial.h>
#include <BaseModule.h>

namespace HogeHoge {
    class SensorModule : public BaseModule {
        // device output
        // none

        // device input
        typedef struct {
            unsigned switch_1: 1;
            unsigned switch_2: 1;
            unsigned switch_3: 1;
            unsigned switch_4: 1;
            unsigned switch_5: 1;
            unsigned switch_6: 1;
            unsigned switch_7: 1;
        }  in_switch_struct;

        typedef union {
            uint8_t all;
            in_switch_struct bits;
        } in_switch;

        in_switch in_swich_state;

        short in_analog_1;
        short in_analog_2;
        short in_analog_3;
        short in_analog_4;
        short in_analog_5;
        short in_analog_6;

        short* const value_map[6];

        /// @brief command transmit function
        /// @param cmd command
        /// @param device_id Device ID
        /// @param length Data length
        /// @param data Data
        /// @return OK
        bool Command(uint8_t cmd, uint8_t device_id, uint8_t length, void* data) override;

    public:
        /// @brief Delete default constructer
        SensorModule() = delete;

        /// @brief Constructer
        /// @param _serial Reference of HogeHogeSerial 
        /// @param module_num Module numbber
        SensorModule(HogeHogeSerial &_serial, uint8_t module_num);

        /// @brief Received data processing function
        /// @param cmd Command
        /// @param device_id Device ID
        /// @param length Size of received data size
        /// @param data received data
        void Receive(uint8_t cmd, uint8_t device_id, uint8_t length, void* data) override;

        bool GetSwitch(uint8_t device_id);
        short GetAnalog(uint8_t device_id);
    };
}