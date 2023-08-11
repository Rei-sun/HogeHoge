#pragma once

#include <chrono>

/**
 * @namespace HogeHoge
*/
namespace HogeGen2 {
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

        // progress time for last loop process [s]
        float process_time;

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
         * @brief Get Process time
         * @return Process time [s]
        */
        float GetProcessTime() {
            return process_time;
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
            // Process elapsed time
            end = std::chrono::system_clock::now();
            process_time = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count() * 0.000001f;
            int elapsed = 0;

            // Sleep
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
