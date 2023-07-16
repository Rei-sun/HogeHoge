#ifndef _H_HOGEHOGE_
#define _H_HOGEHOGE_

#include <Timer.h>
#include <SerialCommunication.h>
#include <HogeHogeSerial.h>

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
        
        /**
         * @brief コンストラクタ
        */
        Hoge() :serial("/dev/ESP32-WROOM-32E") {
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