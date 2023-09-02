#pragma once

#include <Client.h>
#include <Module.h>
#include <IModuleDeserializer.h>
#include <map>
#include <sstream>

namespace HogeGen2 {

class IPCommunicationSub {
    Client client;
    
    static inline std::map<ModuleID, std::vector<IModuleDeserializer*>> deserializer_reference;

    static void Receive(Client* cp, uint8_t* data, int size) {
        static uint8_t buffer[512]; // 過剰なバッファー
        static int buffer_idx = 0;

        for (int i = 0; i < size; i++) {
            buffer[buffer_idx++] = data[i];
        }

        while (1) {
            if (buffer_idx <= 3) return;

            auto data_size = buffer[2] + 3;

            if (buffer_idx <= data_size) return;

            // バッファから取り出す
            uint8_t *msg = new uint8_t[data_size];
            for (int i = 0; i < data_size; i++) {
                msg[i] = buffer[i];
            }

            auto module_id = msg[0];
            auto module_num = msg[1];
            auto module_data_size = msg[2];
            auto module_data = msg + 3;

            auto it = deserializer_reference.find((ModuleID)module_id);
            if (it ==  deserializer_reference.end()) return;
            
            deserializer_reference[(ModuleID)module_id][module_num - 1]->Deserialize((uint8_t *)module_data, module_data_size);

            // バッファを詰める & index reset
            for (int i = 0; i < buffer_idx - data_size; i++) {
                uint8_t val = buffer[data_size + i];
                buffer[data_size + i] = 0;
                buffer[i] = val;
            }
            buffer_idx = buffer_idx - data_size;
        }
    }

public:
    static void RegisterModuleHoge(ModuleID module_id, uint8_t module_num, IModuleDeserializer *deserializer) {
        // if the key has no matching value, add one.
        auto it = deserializer_reference.find(module_id);
        if (it ==  deserializer_reference.end()) deserializer_reference[module_id] = std::vector<IModuleDeserializer*>();

        deserializer_reference[module_id].push_back(deserializer);
    }

    IPCommunicationSub():
        client()
    {}

    void Start(const char* host, int port) {
        client.Init(host, port);
        client.Connect(Receive);
    }

    void SendHoge(BaseModule &module) {
        auto cmd_str = std::string("Hoge");
        auto module_str = module.GetModuleName();
        uint8_t num_str = module.GetModuleNum();
        client.SendStr("%s %s %d", cmd_str.c_str(), module_str.c_str(), num_str);
    }

    void SendFoo(BaseModule &module, uint8_t _device_num, float _value) {
        auto cmd_str = std::string("Foo");
        auto module_str = module.GetModuleName();
        uint8_t module_num = module.GetModuleNum();
        uint8_t device_num = _device_num;
        float value = _value;
        client.SendStr("%s %s %d %d %f", cmd_str.c_str(), module_str.c_str(), module_num, device_num, value);
    }
}; 

}
