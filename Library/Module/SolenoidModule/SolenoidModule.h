#pragma once
#include <HogeHogeSerial.h>
#include <BaseModule.h>
#include <CommandDefinition.h>

namespace HogeHoge {
    class SolenoidModule : public BaseModule {
        typedef struct {
            unsigned state_1: 1;
            unsigned state_2: 1;
            unsigned state_3: 1;
            unsigned state_4: 1;
            unsigned state_5: 1;
            unsigned state_6: 1;
            unsigned state_7: 1;
            unsigned state_8: 1;
        }  in_state_struct;
        
        typedef union {
            uint8_t all;
            in_state_struct bits;
        } in_state;

        // device output
        in_state in_solenoid_state;

        // device input
        // none 

        /// @brief command transmit function
        /// @param cmd command
        /// @param device_id Device ID
        /// @param length Data length
        /// @param data Data
        /// @return OK
        bool Command(uint8_t cmd, uint8_t device_id, uint8_t length, void* data) override;

    public:
        /// @brief Delete default constructer
        SolenoidModule() = delete;

        /// @brief Constructer
        /// @param _serial Reference of HogeHogeSerial
        /// @param module_num Module numbber
        SolenoidModule(HogeHogeSerial &_serial, uint8_t module_num);

        /// @brief batch sending
        void SendBatch() override;

        void SetState(uint8_t device_id, bool state);

        bool GetState(uint8_t device_id);
    };
}