#pragma once

#include <SerialCommunication.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <ios>
#include <sstream>

namespace HogeHoge {
    enum class Module {
        UndefinedModule,
        MotorControlModule,
        EncoderModule,
        SensorModule,
        SolenoidModule
    };

    class HogeHogeSerial : public SerialCommunication {
        const std::string target_device_name;

        // void OnConnect();
        // void OnDisconnect();
        void OnReceive(void*, size_t);
        // void OnDisconnect();
        // void OnReconnected();
        // void OnTimeout();

        HogeHogeSerial(){};

    public:
        HogeHogeSerial(std::string device_name);
        void Send(uint8_t module_id, uint8_t commnad, uint8_t module_num, uint8_t device_id, uint8_t length, void *data);
    };
}