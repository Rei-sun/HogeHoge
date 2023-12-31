#pragma once

#include <Utility.h>
#include <SerialCommunication.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <ios>
#include <sstream>
#include <queue>
#include <vector>

namespace HogeGen2 {
    /// @brief Serial communication for HogeHoge
    class HogeHogeSerial : public SerialCommunication {
        // Target device name
        std::string target_device_name;

        // void OnConnect();
        void OnReceive(void*, size_t);

    public:
        /// @brief Constructer
        HogeHogeSerial();

        /// @brief Start serial communication
        /// @param device_name  Device name (ex. /dev/ttyACM0)
        void Start(std::string device_name);

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