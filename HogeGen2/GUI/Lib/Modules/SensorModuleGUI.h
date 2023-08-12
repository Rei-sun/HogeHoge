#pragma once

#include <Module.h>
#include <IModuleDeserializer.h>

namespace HogeGen2 {

class SensorModuleGUI : public SensorModule, public IModuleDeserializer {
public:
    SensorModuleGUI(uint8_t _module_num);
    void Deserialize(uint8_t* data, int size);
};

}