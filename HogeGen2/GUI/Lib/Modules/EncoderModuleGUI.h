#pragma once

#include <Module.h>
#include <IModuleDeserializer.h>
#include <IGroupBox.h>

namespace HogeGen2 {

class EncoderModuleGUI : public EncoderModule, public IModuleDeserializer, public IGroupBox {
    static inline std::vector<std::string> category_pose_text = {
        "Position X",
        "Position Y",
        "Roll",
        "Pitch",
        "Yaw" 
    };

    static inline std::vector<std::string> category_pulse_text = {
        "Pulse"
    };

    std::vector<QLineEdit*> line_edits;

public:
    EncoderModuleGUI(uint8_t _module_num);
    void Deserialize(uint8_t* data, int size);
    void WidgetUpdate();
    QGroupBox *GetGroupBox();
};

}