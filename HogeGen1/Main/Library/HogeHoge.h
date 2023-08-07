#ifndef _H_HOGEHOGE_
#define _H_HOGEHOGE_

#include <Timer.h>
#include <SerialCommunication.h>
#include <HogeHogeSerial.h>
#include <MotorControlModule.h>
#include <EncoderModule.h>
#include <SensorModule.h>
#include <SolenoidModule.h>

#include <signal.h>

namespace HogeHoge {
    class Hoge {
        // condition
        static bool condition;

        /**
         * @brief Abort handler
        */
        static void abort_handler(int sig) {
            Hoge::condition = false;
            printf("Keyboard interrupted.\n");
        }

        /**
         * @brief Regist Abort handler function
        */
        static void RegisterAbort() {
            signal(SIGINT, Hoge::abort_handler);
        }

        void OnReceive(void* recv_data, size_t recv_size) {
            auto data = (uint8_t*)recv_data;
            
            // Copy to data buffer
            for (int i = 0; i < recv_size; i++) {
                receive_data_buffer.push(data[i]);
            }

            while (1) {
                // If "recv_size" more less then 6, cannot interpreta　message.
                if (receive_data_buffer.size() < 6) break;

                // Read message from data buffer
                for (int i = 0; tmp_buffer.size() < 5; i++) {
                    tmp_buffer.push_back(receive_data_buffer.front());
                    receive_data_buffer.pop();
                }
                uint8_t length = tmp_buffer[4];

                if (receive_data_buffer.size() < length + 1/*checksum*/) break;

                for (int i = 0; tmp_buffer.size() < 5 + length + 1; i++) {
                    tmp_buffer.push_back(receive_data_buffer.front());
                    receive_data_buffer.pop();
                }

                receive_msg_buffer.push(tmp_buffer);
                tmp_buffer.clear();
            }

            for (int i = 0; i < receive_msg_buffer.size(); i++) {
                auto msg = receive_msg_buffer.front();
                receive_msg_buffer.pop();

                uint8_t module_num = msg[2];

                switch (msg[0])
                {
                    case (uint8_t)ModuleID::MotorControlModule:
                        // nothing
                        break;
                    case (uint8_t)ModuleID::EncoderModule:
                        if (module_num == 1) encoder_module_1.Receive(msg[1], msg[3], msg[4], &msg[5]);
                        break;
                    case (uint8_t)ModuleID::SensorModule:
                        if (module_num == 1) sensor_module_1.Receive(msg[1], msg[3], msg[4], &msg[5]);
                        break;
                    case (uint8_t)ModuleID::SolenoidModule:
                        // nothing
                        break;
                    default:
                        break;
                }
            }
        }

    public:
        // Instance for serial communication
        static HogeHogeSerial serial;

        // Motor control module
        static MotorControlModule motor_control_module_1;

        // Encoder module
        static EncoderModule encoder_module_1;

        // Sensor module
        static SensorModule sensor_module_1;
        
        // Solenoid module
        static SolenoidModule solenoid_module_1;

        std::queue<uint8_t> receive_data_buffer;
        std::queue<std::vector<uint8_t>> receive_msg_buffer;
        std::vector<uint8_t> tmp_buffer;

        /**
         * @brief Constructer
        */
        Hoge()
        {
            RegisterAbort();
            serial.RegisterCallbackOnReceive([this](void* data, size_t size){ OnReceive(data, size); });
        }

        /**
         * @brief Condition check function
        */
       static bool Good() {
        return Hoge::condition;
       }
    };
    bool Hoge::condition = true;
}

// HogeHoge::HogeHogeSerial HogeHoge::Hoge::serial("/dev/ESP32-WROOM-32E");
HogeHoge::HogeHogeSerial HogeHoge::Hoge::serial("/dev/ttyACM0");
HogeHoge::MotorControlModule HogeHoge::Hoge::motor_control_module_1(HogeHoge::Hoge::serial, 1);
HogeHoge::EncoderModule HogeHoge::Hoge::encoder_module_1(HogeHoge::Hoge::serial, 1);
HogeHoge::SensorModule HogeHoge::Hoge::sensor_module_1(HogeHoge::Hoge::serial, 1);
HogeHoge::SolenoidModule HogeHoge::Hoge::solenoid_module_1(HogeHoge::Hoge::serial, 1);

// By including this file, instantiate hoge.
HogeHoge::Hoge hoge;

#endif