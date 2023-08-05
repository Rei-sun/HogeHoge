#pragma once

#include <Server.h>
#include <HogeHoge.h>

using namespace HogeHoge;

class GUICommunication {
    Server server;
    Hoge hoge;
public:
    static void Receive(Server* sp, char *data, int data_size, int client_fd) {
        std::string recv(data);
        auto cmd = GUICommunication::split(recv, ' ');

        if (cmd.size() <= 2) return;
        if (cmd[0] != "Hoge") return;

        auto num = stoi(cmd[2]);

        if (cmd[1] == "EncoderModule") {
            if (num == 1) {
                auto serialized = Hoge::encoder_module_1.Serialized();
                sp->SendData(0, serialized.second.get(), serialized.first);
                //printf("Send\n");
            }
        } else if (cmd[1] == "MotorControlModule") {
            if (num == 1) {
                auto serialized = Hoge::motor_control_module_1.Serialized();
                sp->SendData(0, serialized.second.get(), serialized.first);
            }
        } else if (cmd[1] == "SensorModule") {
            if (num == 1) {
                auto serialized = Hoge::sensor_module_1.Serialized();
                sp->SendData(0, serialized.second.get(), serialized.first);
            }
        } else if (cmd[1] == "SolenoidModule") {
            if (num == 1) {
                auto serialized = Hoge::solenoid_module_1.Serialized();
                sp->SendData(0, serialized.second.get(), serialized.first);
            }
        }
        
    }

    GUICommunication(int _port) :
        server()
    {
        server.Init(_port, GUICommunication::Receive);
    }

    ~GUICommunication() {
        server.Close();
    }

    static std::vector<std::string> split(std::string str, char separator) {
        std::istringstream iss(str);
        std::string s;
        std::vector<std::string> ret;
        while (std::getline(iss, s, separator)) {
            ret.push_back(s);
        }
        return ret;
    }
};