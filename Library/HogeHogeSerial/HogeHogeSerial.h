#pragma once

#include <SerialCommunication.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <ios>
#include <sstream>
#include <queue>
#include <vector>

namespace HogeHoge {
    /// @brief Module ID
    enum class ModuleID {
        UndefinedModule,
        MotorControlModule,
        EncoderModule,
        SensorModule,
        SolenoidModule
    };

    /// @brief Serial communication for HogeHoge
    class HogeHogeSerial : public SerialCommunication {
        // Target device name
        const std::string target_device_name;

        // void OnConnect();
        // void OnDisconnect();
        void OnReceive(void*, size_t);
        // void OnDisconnect();
        // void OnReconnected();
        // void OnTimeout();

        /// @brief Constructer
        HogeHogeSerial(){};

    public:
        /// @brief Constructer
        /// @param device_name Device name (ex. /dev/ttyACM0)
        HogeHogeSerial(std::string device_name);

        /// @brief Transmit command function
        /// @param module_id Module ID
        /// @param commad Commad
        /// @param module_num Module Number
        /// @param device_id Device ID
        /// @param length Data length
        /// @param data Data
        void Send(uint8_t module_id, uint8_t commad, uint8_t module_num, uint8_t device_id, uint8_t length, void *data);
    };
}