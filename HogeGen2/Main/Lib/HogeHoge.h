#ifndef _H_HOGEHOGE_
#define _H_HOGEHOGE_

#include <SerialCommunication.h>
#include <HogeHogeSerial.h>
#include <ModuleManagerMain.h>
#include <CommandDefinition.h>
#include <Timer.h>

#include <unistd.h>
#include <signal.h>
#include <functional>
#include <vector>
#include <queue>

namespace HogeGen2 {
    class Hoge {
        // condition
        inline static bool condition = false;

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

        static void OnConnect() {
            std::queue<uint8_t> empty1;
            std::swap(receive_data_buffer, empty1);

            // Buffer of msg
            std::queue<std::vector<uint8_t>> empty2;
            std::swap(receive_msg_buffer, empty2);
        
            tmp_buffer.clear();

            std::cout << "Receive buffer clear." << std::endl;
        }

        static void OnReconnect() {
            OnConnect();    
        }

        static void OnReceive(void* recv_data, size_t recv_size) {
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

                ReceiveCommand(msg[0], msg[1], msg[2], msg[3], msg[4], &msg[5]);
            }
        }

        static void ReceiveCommand(uint8_t module_id, uint8_t cmd, uint8_t module_num, uint8_t dev_id, uint8_t length, void *data) {
            //printf("%x, %x, %x, %x\n", module_id, cmd, module_num, dev_id);
            
            if (module_id == (uint8_t)ModuleID::EncoderModule) {

                // Check Module number
                if (ModuleManagerMain::encoderModules.size() < module_num) return;
                
                ModuleManagerMain::encoderModules[module_num - 1]->ReceiveCommand(cmd, dev_id, length, data);

            } else if (module_id == (uint8_t)ModuleID::SensorModule) {
                
                // Check Module number
                if (ModuleManagerMain::sensorModules.size() < module_num) return;
                
                ModuleManagerMain::sensorModules[module_num - 1]->ReceiveCommand(cmd, dev_id, length, data);

            } else {

                // nothing

            }
        }

        // Buffer of receive data from serial
        inline static std::queue<uint8_t> receive_data_buffer;

        // Buffer of msg
        inline static std::queue<std::vector<uint8_t>> receive_msg_buffer;
        
        // temporary buffer
        inline static std::vector<uint8_t> tmp_buffer;

        // vector for sensor value request function 
        inline static std::vector<std::function<void()>> requestFunc;

        // vector for set actuator control function
        inline static std::vector<std::function<void()>> batchFunc;

    public:
        // Instance for serial communication
        inline static HogeHogeSerial serial;

        /**
         * @brief Constructer
        */
        Hoge() {}

        static void Init() {
            printf("pid = %d\n", getpid());
            auto new_prio = nice(-20);
            if (new_prio == -1) perror("nice");
            else printf("priolity = %d\n", new_prio);
            ModuleManagerMain::SetModule<EncoderModuleMain>(1);
            ModuleManagerMain::SetModule<MotorModuleMain>(1);
            ModuleManagerMain::SetModule<SensorModuleMain>(1);
            ModuleManagerMain::SetModule<SolenoidModuleMain>(1);
            
            RegisterAbort();
            serial.RegisterCallbackOnConnect(OnConnect);
            serial.RegisterCallbackOnReceive(OnReceive);
            serial.RegisterCallbackOnReconnect(OnReconnect);
            serial.Start("/dev/ESP32-WROOM-32E");
            condition = true;
        }

        /// @brief Condition check function
        static bool Good() { return Hoge::condition; }

        /// @brief To request sensor value
        static void GetSensorValue() { for (auto f : requestFunc) f(); }

        /// @brief To set actuator contol
        static void SetActuatorControl() { for (auto f : batchFunc) f(); }

        /// @brief register request function
        /// @param func 
        static void RegisterRequestSensor(std::function<void ()> func) { requestFunc.push_back(func); }

        /// @brief register set actuator function
        /// @param func 
        static void RegisterBatchSender(std::function<void()> func) { batchFunc.push_back(func); }
    };
}
#endif