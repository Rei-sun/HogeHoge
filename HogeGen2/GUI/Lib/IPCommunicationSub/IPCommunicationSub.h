#include <Client.h>
#include <Module.h>
#include <IModuleDeserializer.h>
#include <map>

namespace HogeGen2 {

class IPCommunicationSub {
    Client client;
    
    static inline std::map<ModuleID, std::vector<IModuleDeserializer*>> deserializer_reference;

    static void Receive(Client* cp, char* data, int size) {
        if (size < 3) return;
        auto module_id = data[0];
        auto module_num = data[1];
        auto module_data = data + 2;

        auto it = deserializer_reference.find((ModuleID)module_id);
        if (it !=  deserializer_reference.end()) return;
        
        deserializer_reference[(ModuleID)module_id][module_num - 1]->Deserialize((uint8_t *)module_data, size - 2);
    }

    static void RegisterModuleHoge(ModuleID module_id, uint8_t module_num, IModuleDeserializer *deserializer) {
        // if the key has no matching value, add one.
        auto it = deserializer_reference.find(module_id);
        if (it ==  deserializer_reference.end()) deserializer_reference[module_id] = std::vector<IModuleDeserializer*>();

        deserializer_reference[module_id].push_back(deserializer);
    }

    IPCommunicationSub(const char* host, int port):
        client()
    {
        client.Init(host, port);
    }

    void Start() {
        client.Connect(Receive);
    }
}; 

}
