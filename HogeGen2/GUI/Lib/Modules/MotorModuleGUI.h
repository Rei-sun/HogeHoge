#pragma once

#include <Module.h>
#include <IModuleDeserializer.h>

namespace HogeGen2 {

class MotorModuleGUI : public MotorModule, public IModuleDeserializer {
public:
    MotorModuleGUI(uint8_t _module_num);
    void Deserialize(uint8_t* data, int size);
};

}