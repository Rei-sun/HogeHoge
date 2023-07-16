#ifndef _H_HOGEHOGE_
#define _H_HOGEHOGE_

#include <Timer.h>
#include <SerialCommunication.h>
#include <HogeHogeSerial.h>
#include <MotorControlModule.h>

#include <signal.h>

namespace HogeHoge {
    class Hoge {
        // 状態
        static bool condition;

        /**
         * @brief Abort handler
        */
        static void abort_handler(int sig) {
            Hoge::condition = false;
        }

        /**
         * @brief abot handler 登録関数
        */
        void RegisterAbort() {
            signal(SIGINT, Hoge::abort_handler);
        }

    public:
        // 通信用インスタンス
        HogeHogeSerial serial;
        // MotorControlModule
        MotorControlModule motor_control_module_1;
        
        /**
         * @brief コンストラクタ
        */
        Hoge() :
            serial("/dev/ESP32-WROOM-32E"),
            motor_control_module_1(serial, 1)
        {
            RegisterAbort();
        }

        /**
         * @brief 状態確認関数
        */
       static bool Good() {
        return Hoge::condition;
       }
    };
    bool Hoge::condition = true;
}


HogeHoge::Hoge hoge;
#endif