#ifndef _H_HOGEHOGE_
#define _H_HOGEHOGE_

#include <Timer.h>
#include <SerialCommunication.h>
#include <HogeHogeSerial.h>
#include <MotorControlModule.h>
#include <EncoderModule.h>

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
        }

        /**
         * @brief Regist Abort handler function
        */
        static void RegisterAbort() {
            signal(SIGINT, Hoge::abort_handler);
        }

    public:
        // Instance for serial communication
        static HogeHogeSerial serial;

        // Motor control module
        static MotorControlModule motor_control_module_1;

        // Encoder Module
        static EncoderModule encoder_module_1;
        
        /**
         * @brief Constructer
        */
        Hoge()
        {
            RegisterAbort();
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

HogeHoge::HogeHogeSerial HogeHoge::Hoge::serial("/dev/ESP32-WROOM-32E");
HogeHoge::MotorControlModule HogeHoge::Hoge::motor_control_module_1(HogeHoge::Hoge::serial, 1);
HogeHoge::EncoderModule HogeHoge::Hoge::encoder_module_1(HogeHoge::Hoge::serial, 1);

// By including this file, instantiate hoge.
HogeHoge::Hoge hoge;

#endif