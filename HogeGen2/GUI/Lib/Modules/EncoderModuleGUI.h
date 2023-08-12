#pragma once

#include <Module.h>
#include <IModuleDeserializer.h>

namespace HogeGen2 {

class EncoderModuleGUI : public EncoderModule, public IModuleDeserializer {
public:
    EncoderModuleGUI(uint8_t _module_num);
    void Deserialize(uint8_t* data, int size);
};

}