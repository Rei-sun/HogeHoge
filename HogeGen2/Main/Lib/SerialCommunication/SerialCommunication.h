#pragma once

// Usage？
// HogeHoge::SerialCommunication serial;
// serial.RegisterCallbackOnConnect([](){ printf("いぇーい、接続できたよぉぉぉ！\n"); });
// serial.RegisterCallbackOnDisconnect([](){ printf("いぇーい、切断したよぉぉぉ！\n"); });
// serial.RegisterCallbackOnReceive([](void* data, size_t size){ printf("Recv %ld byte, %s\n", size, (char *)data); });
// serial.RegisterCallbackOnDisconnected([](){ printf("接続が切れましたが？\n"); });
// serial.RegisterCallbackOnReconnected([](){ printf("いぇーい、再接続されましたぁぁぁ!\n"); });
// serial.RegisterCallbackOnTimeout([](){ printf("タイムアウトしたって\n"); });

// // Start communication
// std::string target = "/dev/ESP32-WROOM-32E";
// if (!serial.Open(target, false)) {
//     std::cout << "Cannot connect " << target << std::endl;
//     std::quick_exit(0);
// }

#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <functional>
#include <thread>
#include <memory>
#include <sys/ioctl.h>
#include <mutex>

/**
 * @namespace HogeGen2
*/
namespace HogeGen2 {
    /**
     * @brief Class for serial communication
     * @details During the connection, terminate the connection when the instance is destroyed
    */
    class SerialCommunication {
        // Target device
        std::string device_name;
        
        // File discripter for communication
        int fd;

        // Connect callback
        std::function<void(void)> on_connect;

        // Close callback
        std::function<void(void)> on_close;

        // Receive callback
        std::function<void(void*, size_t)> on_receive;

        // Disconnected callback
        std::function<void(void)> on_disconnect;

        // Reconnect callback
        std::function<void(void)> on_reconnect;

        // Timeout callback
        std::function<void(void)> on_timeout;

        // thread for communication
        std::shared_ptr<std::thread> thread;

        // Flag for thread control
        bool thread_continue;

        // Flag for connected
        bool is_connected;

        // transmit failure counter
        uint32_t transmit_failure_count;

        // Setting for Timeout [ms]
        uint32_t timeout;

        // timeout [s]
        time_t timeout_sec;

        // timeout [us]
        suseconds_t timeout_usec;

        // Reconnect interval [s]
        uint32_t reconnect_interval;

        // For Exclusive Control
        std::mutex io_mutex;

    private:
        /**
         * @brief Start thread function
        */
        void StartCommunicationThread();

        /**
         * @brief Stop thread function
        */
        void StopCommunicationThread();

        /**
         * @brief Communication thread
        */
        void CommunicationProcess();

        /**
         * @brief Start thread for internal function
         * @param device_name Target device name
         * @param reconnect is reconnect?
         * @return Success or Failure
        */
        bool OpenSerialPort(const char *device_name, bool reconnect);

        /**
         * @brief Stop thread for internal function
         * @param error calling by error?
        */
        void CloseSerialPort(bool error);

    public:
        /**
         * @brief Constructer
        */
        SerialCommunication();

        /**
         * @brief Destructer
        */
        ~SerialCommunication();

        /**
         * @brief Start Communication function
         * @param device_name Target device name
         * @return Success or Failure
        */
        bool Open(const char *device_name, bool thread_start);

        /**
         * @brief Start Communication function
         * @param device_name Target device name
         * @return Success or Failure
        */
        bool Open(std::string device_name, bool thread_start);

        /**
         * @brief Close communication function
        */
        void Close();

        /**
         * @brief Close communication function
         * @return connect or not connect
        */
        bool IsConnect();

        /**
         * @brief Transmit function
         * @param data Data
         * @param size Size for transmit data
        */
        void Transmit(uint8_t *data, size_t size);

        /**
         * @brief Transmit function
         * @param msg Data
         * @param size Size for transmit data
        */
        void Transmit(const char* msg, size_t size);
        
        /**
         * @brief Transmit functinon
         * @param data Data
         * @param size size for transmit data
        */
        void Transmit(std::string msg);

        /**
         * @brief Callback registration function for OnConnect
         * @param callback Callback function
        */
        void RegisterCallbackOnConnect(std::function<void(void)> callback);

        /**
         * @brief Callback registration function for OnClose
         * @param callback Callback function
        */
        void RegisterCallbackOnClose(std::function<void(void)> callback);

        /**
         * @brief Callback registration function for OnReceive
         * @param callback Callback function
        */
        void RegisterCallbackOnReceive(std::function<void(void*, size_t)> callback);

        /**
         * @brief Callback registration function for OnDisconnect
         * @param callback Callback function
        */
        void RegisterCallbackOnDisconnect(std::function<void(void)> callback);

        /**
         * @brief Callback registration function for OnReconnect
         * @param callback Callback function
        */
        void RegisterCallbackOnReconnect(std::function<void(void)> callback);

        /**
         * @brief Callback registration function for OnTimeout
         * @param callback Callback function
        */
        void RegisterCallbackOnTimeout(std::function<void(void)> callback);
    };
}