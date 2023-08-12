#pragma once

#include <Module.h>
#include <IModuleDeserializer.h>

namespace HogeGen2 {

class SolenoidModuleGUI : public SolenoidModule, public IModuleDeserializer {
public:
    SolenoidModuleGUI(uint8_t _module_num);
    void Deserialize(uint8_t* data, int size);
};

}
