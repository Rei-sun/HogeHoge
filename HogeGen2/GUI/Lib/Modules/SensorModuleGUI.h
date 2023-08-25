#pragma once

#include <Module.h>
#include <IModuleDeserializer.h>
#include <IGroupBox.h>

namespace HogeGen2 {

class SensorModuleGUI : public SensorModule, public IModuleDeserializer, public IGroupBox {
    static inline std::vector<std::string> category_analog_text = {
        "Analog"
    };

    static inline std::vector<std::string> category_digital_text = {
        "Digital"
    };
public:
    SensorModuleGUI(uint8_t _module_num);
    void Deserialize(uint8_t* data, int size);
    QGroupBox *GetGroupBox();
};

}