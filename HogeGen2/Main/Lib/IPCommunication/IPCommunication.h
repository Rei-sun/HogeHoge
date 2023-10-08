#pragma once

#include <CommandDefinition.h>
#include <Server.h>
#include <IModuleSerializer.h>
#include <IModuleController.h>
#include <ModuleComposition.h>
#include <map>
#include <sstream>

using namespace HogeGen2;

class IPCommunication {
    Server server;
    static inline std::map<std::string, std::vector<IModuleSerializer*>> serializer_reference;
    static inline std::map<std::string, std::vector<IModuleController*>> controller_reference;
public:
    IPCommunication() :
        server()
    {}

    ~IPCommunication() {
        server.Close();
    }

    void Start(int _port) {
        server.Init(_port, IPCommunication::Receive);
    }

    static void RegisterModuleSerialize(std::string key, IModuleSerializer* p) {
        // if the key has no matching value, add one.
        auto it = serializer_reference.find(key);
        if (it == serializer_reference.end()) serializer_reference[key] = std::vector<IModuleSerializer*>();

        serializer_reference[key].push_back(p);
    }

    static void RegisterModuleControl(std::string key, IModuleController* p) {
        // if the key has no matching value, add one.
        auto it = controller_reference.find(key);
        if (it ==  controller_reference.end()) controller_reference[key] = std::vector<IModuleController*>();

         controller_reference[key].push_back(p);
    }

    static void Receive(Server* sp, char *data, int data_size, int client_fd) {
        std::string recv(data);
        auto cmd = IPCommunication::split(recv, ' ');

        // If cmd.size < 3, exit function
        if (cmd.size() < 3) return;
        
        // Command "Hoge" : Hoge <Module name> <Module number>
        if (cmd[0] == "Hoge") {

            // Check if the key exists
            auto it = serializer_reference.find(cmd[1]);
            if (it == serializer_reference.end()) return;

            // Cache by reference
            auto &v = serializer_reference[cmd[1]];

            // Check number
            auto num = std::stoi(cmd[2]);
            if (v.size() > num) return;
        
            // Send
            auto serialized =  v[num - 1]->Serialized();
            sp->SendData(client_fd, serialized.second.get(), serialized.first);
        
        // Command "Foo" : Foo <Module name> <Module number> <Device ID> <value>
        } else if (cmd[0] == "Foo") {

            // If cmd.size < 5, exit function
            if (cmd.size() < 5) return;

            // Check if the key exists
            auto it = controller_reference.find(cmd[1]);
            if (it == controller_reference.end()) return;

            // Cache by reference
            auto &v = controller_reference[cmd[1]];

            // Check number
            auto num = std::stoi(cmd[2]);
            if (v.size() > num) return;
        
            // Control
            auto dev_id = std::stoi(cmd[3]);

            // ここでよく例外が発生するので、対処
            auto value = 0.f;
            // doubleで変換してからfloatに変換すると安全らしい
            auto temp = std::stod(cmd[4]);
            value = temp;

            v[num - 1]->Control(dev_id, value);

        } else if (cmd[0] == "Bar") {
            uint8_t tx_size = 3 + (cmd.size() - 1) * 2;
            uint8_t tx_data[tx_size];
            tx_data[0] = (uint8_t)ModuleID::UndefinedModule;
            tx_data[1] = (uint8_t)IPCMD::ModuleCount;
            tx_data[2] = (cmd.size() - 1) * 2;
            for (int i = 1; i < cmd.size(); i++) {
                auto module_id = std::stoi(cmd[i]);
                tx_data[3 + (i - 1) * 2] = module_id;
                tx_data[3 + (i - 1) * 2 + 1] = module_composition.GetCount((ModuleID)module_id);
            }
            sp->SendData(client_fd, tx_data, tx_size);

        }
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