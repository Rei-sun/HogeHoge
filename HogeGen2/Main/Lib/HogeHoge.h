#ifndef _H_HOGEHOGE_
#define _H_HOGEHOGE_

#include <SerialCommunication.h>
#include <IPCommunication.h>
#include <HogeHogeSerial.h>
#include <ModuleManagerMain.h>
#include <CommandDefinition.h>
#include <Timer.h>
#include <ConfigFileLoader.h>

#include <unistd.h>
#include <signal.h>
#include <functional>
#include <vector>
#include <queue>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <MessageOutputter.h>

namespace HogeGen2 {
    class Hoge {
        typedef struct {
            pid_t pid;
            std::string name;
        } ProcessID;

        // sub process list
        inline static std::vector<ProcessID> pids;
        
        // condition
        inline static bool condition = false;

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

        // instance for ip communication
        inline static IPCommunication ip_communication = IPCommunication();

        /// @brief Abort handler
        static void abort_handler(int sig) {
            Hoge::condition = false;
            printf("Keyboard interrupted.\n");
        }

        /// @brief child handler
        static void child_handler(int sig){
            pid_t pid;
            int stat;
            pid = wait(&stat);

            std::cout << "Handle catch pid = " << pid << std::endl;

            if (pid == 0)
                return;
            else if (pid == -1)
                return;

            for(auto it = pids.begin(); it != pids.end();){
                if(pid == (*it).pid){
                    std::cout << "[" << (*it).name << "]" << " Return " << stat << std::endl;
                    pids.erase(it);
                    return;
                }else{
                    it++;
                }
            }
        }

        /// @brief process fork process
        static void LaunchGUIProess() {
            ProcessID process_pid;
            process_pid.pid = fork();

            if (process_pid.pid == -1) {
                
                // フォーク失敗
                std::cerr << "fork error." << std::endl;
                return;

            }
            
            if (process_pid.pid == 0) {
                
                // 子プロセス
                std::string arg_port = "--port=" + std::to_string(ConfigFileLoader::config.gui_server_port);
                std::string path = HOGE_ROOT "GUI/build/App/GUI";
                if(execl(path.c_str(), path.c_str(), "--mode=1", arg_port.c_str(), nullptr) != 0) {
                    perror("execl");
                    exit(1);
                }

            } else {
            
                // 親プロセス
                if ( signal(SIGCHLD, child_handler) == SIG_ERR ) {
                    exit(1);
                }
                process_pid.name = "GUI";
                pids.push_back(process_pid);
            
            }
        }

        /// @brief Regist Abort handler function
        static void RegisterAbort() {
            signal(SIGINT, Hoge::abort_handler);
        }

        /// @brief Regist Connect callback function
        static void OnConnect() {
            std::queue<uint8_t> empty1;
            std::swap(receive_data_buffer, empty1);

            // Buffer of msg
            std::queue<std::vector<uint8_t>> empty2;
            std::swap(receive_msg_buffer, empty2);
        
            tmp_buffer.clear();

            std::cout << "Receive buffer clear." << std::endl;
        }

        /// @brief Regist Reconnect callback function
        static void OnReconnect() {
            OnConnect();    
        }

        /// @brief Regist Receive callback function
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

        /// @brief Received command process function
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

        static void InitLog() {
            std::stringstream ss;
            auto now_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            ss << "HogeHoge_" << std::put_time(localtime(&now_time), "%Y-%m-%d_%H%M%S") << ".log";
            log_output.Init(ss.str());
        }

        static void SetPriority() {
            auto new_prio = nice(-20);
            if (new_prio == -1) {
                log_output.WarnMessage("nice: %s", std::strerror(errno));
            } else {
                log_output.InfoMessage("Process priolity = %d\n", new_prio);
            }
        }

    public:
        // Instance for serial communication
        inline static HogeHogeSerial serial;

        static void Init() {
            InitLog();

            log_output.InfoMessage("PID = %d", getpid());

            log_output.DebugMessage("Load config file.");
            ConfigFileLoader::LoadConfig();

            if (!ConfigFileLoader::IsConfigLoaded()) {
                log_output.ErrorMessage("Config file load failed: %s\n", ConfigFileLoader::config_filename.c_str());
                return;
            }

            SetPriority();

            ModuleManagerMain::SetModule<EncoderModuleMain>(1);
            ModuleManagerMain::SetModule<MotorModuleMain>(1);
            ModuleManagerMain::SetModule<SensorModuleMain>(1);
            ModuleManagerMain::SetModule<SolenoidModuleMain>(1);
            
            RegisterAbort();
            
            serial.RegisterCallbackOnConnect(OnConnect);
            serial.RegisterCallbackOnReceive(OnReceive);
            serial.RegisterCallbackOnReconnect(OnReconnect);
            serial.Start(ConfigFileLoader::config.target_device_name);

            ip_communication.Start(ConfigFileLoader::config.gui_server_port);

            if (ConfigFileLoader::config.use_gui) {
                LaunchGUIProess();
            }

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

        /// @brief register IModuleSerializer for IP
        /// @param key key
        /// @param p IModuleSerializer pointer
        static void RegisterIPSerialize(std::string key, IModuleSerializer* p) { IPCommunication::RegisterModuleSerialize(key, p); }

        /// @brief register IModuleController for IP
        /// @param key key
        /// @param p IModuleController pointer
        static void RegisterIPControl(std::string key, IModuleController* p) { IPCommunication::RegisterModuleControl(key, p); }
    };
}
#endif