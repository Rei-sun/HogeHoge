#include <chrono>

/**
 * @namespace HogeHoge
*/
namespace HogeHoge {
    /**
     * @brief 時間を制御するクラス
    */
    class Timer {
        
        // 周期 [us]
        int time_period;

        // 測定開始時間
        std::chrono::system_clock::time_point start;
        
        // 測定終了時間
        std::chrono::system_clock::time_point end;

        // オーバータイム
        int over_time;

        // 前回のループからの経過時間 [s]
        float delta_time;

    public:
        /**
         * @brief コンストラクタ
        */
        Timer() :
            time_period(0),
            delta_time(0.0f),
            over_time(0)
        {

        }

        /**
         * @brief 前回のループからの経過時間取得関数
         * @return 経過時間 [s]
        */
        float GetDeltaTime() {
            return delta_time;
        }

        /**
         * @brief ループ頻度設定関数
         * @param hz 周波数 [Hz]
        */
        void SetHz(float hz) {
            // 周期[us] = 1000000 [us] / hz [Hz]
            time_period = 1000000.0f / hz;
            start = std::chrono::system_clock::now();
        }

        /**
         * @brief スリープ関数
        */
        void Sleep() {
            int elapsed = 0.f;
            end = std::chrono::system_clock::now();
            elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
            while(elapsed < time_period - over_time) {
                end = std::chrono::system_clock::now();
                elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
                delta_time = elapsed * 0.000001f;
            }
            over_time = elapsed - (time_period - over_time);
            start = end;
        }
    };
}
