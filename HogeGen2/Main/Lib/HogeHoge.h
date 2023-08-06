#ifndef _H_HOGEHOGE_
#define _H_HOGEHOGE_

#include <SerialCommunication.h>
#include <HogeHogeSerial.h>
#include <ModuleManager.h>
#include <CommandDefinition.h>
#include <Timer.h>

#include <unistd.h>
#include <signal.h>

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
            if (module_id == (uint8_t)ModuleID::MotorModule) {
                
                // nothing

            } else if (module_id == (uint8_t)ModuleID::EncoderModule) {

                // Check Device ID
                if (ModuleManager::IsNotValidModuleNumber<EncoderModule>(module_num)) return;
                auto module = ModuleManager::GetEncoderModules()[module_num - 1];

                // WaitForResponse Flag Set
                module->SetWaitForResponse(false);

                // Conditional split for each command.
                if (cmd == (uint8_t)CMD_EncoderModule::GetLocalization) {
                    auto pose_array = (float*)data;
                    module->SetPose(1, pose_array[0]);
                    module->SetPose(2, pose_array[0]);
                    module->SetPose(3, pose_array[0]);
                    module->SetPose(4, pose_array[0]);
                    module->SetPose(5, pose_array[0]);
                } else if (cmd == (uint8_t)CMD_EncoderModule::GetAllPulse) {
                    auto pulse_array = (short*)data;
                    for (int i = 0; i < 4; i++) {
                        module->SetPulse(i + 1, pulse_array[i]);
                    }
                } else {
                    // printf("undefined command\n");
                }

            } else if (module_id == (uint8_t)ModuleID::SensorModule) {
                
                // Check Device ID
                if (ModuleManager::IsNotValidModuleNumber<SensorModule>(module_num)) return;
                auto module = ModuleManager::GetSensorModules()[module_num - 1];

                // WaitForResponse Flag Set
                module->SetWaitForResponse(false);
            
                // Conditional split for each command.
                if (cmd == (uint8_t)CMD_SensorModule::GetSensorData) {
                    auto uint8_data = (uint8_t*)data;
                    auto short_array = (short*)((uint8_t*)data + 1);
                    module->GetDigitalArray()[0].all = uint8_data[0];
                    for (int i = 0; i < 4; i++) {
                        module->SetAnalog(i + 1, short_array[i]);
                    }
                } else {
                    // printf("undefined command\n");
                }

            } else if (module_id == (uint8_t)ModuleID::SolenoidModule) {

                // nothing

            } else {

                // nothing

            }
        }

        static void SendEncoderCommand(bool response, uint8_t module_num, uint8_t cmd, uint8_t device_id, uint8_t length, void* data) {
            if(!serial.IsConnect()) return;
    
            if (response) ModuleManager::GetEncoderModules()[module_num - 1]->SetWaitForResponse(true);
    
            serial.Send((uint8_t)ModuleID::EncoderModule, cmd, module_num, device_id, length, data);

            if (!response) return;

            while (ModuleManager::GetEncoderModules()[module_num - 1]->GetWaitForResponse() && Good()) {
                continue;
            }
        }

        static void SendSensorCommand(bool response, uint8_t module_num, uint8_t cmd, uint8_t device_id, uint8_t length, void* data) {
            if(!serial.IsConnect()) return;
    
            if (response) ModuleManager::GetSensorModules()[module_num - 1]->SetWaitForResponse(true);
    
            serial.Send((uint8_t)ModuleID::SensorModule, cmd, module_num, device_id, length, data);

            if (!response) return;

            while (ModuleManager::GetSensorModules()[module_num - 1]->GetWaitForResponse() && Good()) {
                continue;
            }
        }

        static void SendMotorCommand(bool response, uint8_t module_num, uint8_t cmd, uint8_t device_id, uint8_t length, void* data) {
            if(!serial.IsConnect()) return;
    
            if (response) ModuleManager::GetMotorModules()[module_num - 1]->SetWaitForResponse(true);
    
            serial.Send((uint8_t)ModuleID::MotorModule, cmd, module_num, device_id, length, data);

            if (!response) return;

            while (ModuleManager::GetSensorModules()[module_num - 1]->GetWaitForResponse() && Good()) {
                continue;
            }
        }

        static void SendSolenoidCommand(bool response, uint8_t module_num, uint8_t cmd, uint8_t device_id, uint8_t length, void* data) {
            if(!serial.IsConnect()) return;
    
            if (response) ModuleManager::GetSolenoidModules()[module_num - 1]->SetWaitForResponse(true);
    
            serial.Send((uint8_t)ModuleID::SolenoidModule, cmd, module_num, device_id, length, data);

            if (!response) return;

            while (ModuleManager::GetSolenoidModules()[module_num - 1]->GetWaitForResponse() && Good()) {
                continue;
            }
        }

        // Buffer of receive data from serial
        inline static std::queue<uint8_t> receive_data_buffer;

        // Buffer of msg
        inline static std::queue<std::vector<uint8_t>> receive_msg_buffer;
        
        // temporary buffer
        inline static std::vector<uint8_t> tmp_buffer;

    public:
        // Instance for serial communication
        inline static HogeHogeSerial serial;


        /**
         * @brief Constructer
        */
        Hoge() {}

        static void Init() {
            auto new_prio = nice(-20);
            if (new_prio == -1) perror("nice");
            else printf("priolity = %d\n", new_prio);
            ModuleManager::MakeEncoderModule(1);
            ModuleManager::MakeSensorModule(1);
            ModuleManager::MakeMotorModule(1);
            ModuleManager::MakeSolenoidModule(1);
            RegisterAbort();
            serial.RegisterCallbackOnConnect(OnConnect);
            serial.RegisterCallbackOnReceive(OnReceive);
            serial.RegisterCallbackOnReconnect(OnReconnect);
            serial.Start("/dev/ESP32-WROOM-32E");
            condition = true;
        }

        /**
         * @brief Condition check function
        */
        static bool Good() {
            return Hoge::condition;
        }

        static void GetSensorValue() {
            if (ModuleManager::GetEncoderModules().size() > 0) {
                SendEncoderCommand(true, 1, (uint8_t)CMD_EncoderModule::GetLocalization, 0, 0, nullptr);
            }
            for (int i = 0; i < ModuleManager::GetEncoderModules().size(); i++) {
                SendEncoderCommand(true, i + 1, (uint8_t)CMD_EncoderModule::GetAllPulse, 0, 0, nullptr);
            }
            for (int i = 0; i < ModuleManager::GetSensorModules().size(); i++) {
                SendSensorCommand(true, i + 1, (uint8_t)CMD_SensorModule::GetSensorData, 0, 0, nullptr);
            }
        }

        static void SetActuatorControl() {
            for (int i = 0; i < ModuleManager::GetMotorModules().size(); i++) {
                SendMotorCommand(false, i + 1, (uint8_t)CMD_MotorModule::SetAllDuty, 0, sizeof(float) * 6, ModuleManager::GetMotorModules()[i]->GetDutyArray());
            }
            for (int i = 0; i < ModuleManager::GetSolenoidModules().size(); i++) {
                SendSolenoidCommand(false, i + 1, (uint8_t)CMD_SolenoidModule::SetAllState, 0, sizeof(uint8_t) * 1, &ModuleManager::GetSolenoidModules()[i]->GetStateArray()->all);
            }
        }
    };
}
#endif