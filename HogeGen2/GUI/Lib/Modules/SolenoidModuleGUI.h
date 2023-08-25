#pragma once

#include <Module.h>
#include <IModuleDeserializer.h>
#include <IGroupBox.h>

namespace HogeGen2 {

class SolenoidModuleGUI : public SolenoidModule, public IModuleDeserializer, public IGroupBox {
    static inline std::vector<std::string> category_solenoid_text = {
        "Solenoid"
    };
public:
    SolenoidModuleGUI(uint8_t _module_num);
    void Deserialize(uint8_t* data, int size);
    QGroupBox *GetGroupBox();
};

}
