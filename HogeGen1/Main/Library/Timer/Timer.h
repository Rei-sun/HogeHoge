#pragma once

#include <chrono>

/**
 * @namespace HogeHoge
*/
namespace HogeHoge {
    /**
     * @brief Timer for loop
    */
    class Timer {
        
        // Timer period [us]
        int time_period;

        // start measurement time point
        std::chrono::system_clock::time_point start;
        
        // end measurement time point
        std::chrono::system_clock::time_point end;

        // over time
        int over_time;

        // progress time for last loop [s]
        float delta_time;

    public:
        /**
         * @brief Constructer
        */
        Timer() :
            time_period(0),
            delta_time(0.0f),
            over_time(0)
        {

        }

        /**
         * @brief Get Delta time
         * @return Delta time [s]
        */
        float GetDeltaTime() {
            return delta_time;
        }

        /**
         * @brief Set loop rate functino
         * @param hz frequency [Hz]
        */
        void SetHz(float hz) {
            // period [us] = 1000000 [us] / hz [Hz]
            time_period = 1000000.0f / hz;
            start = std::chrono::system_clock::now();
        }

        /**
         * @brief Sleep function
        */
        void Sleep() {
            int elapsed = 0.f;
            do {
                end = std::chrono::system_clock::now();
                elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
                delta_time = elapsed * 0.000001f;
            } while (elapsed < time_period - over_time);
            over_time = elapsed - (time_period - over_time);
            start = end;
        }
    };
}
