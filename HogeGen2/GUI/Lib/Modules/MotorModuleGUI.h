#pragma once

#include <Module.h>
#include <IModuleDeserializer.h>
#include <IGroupBox.h>
#include <QLineEdit>

namespace HogeGen2 {

class MotorModuleGUI : public MotorModule, public IModuleDeserializer, public IGroupBox {
    static inline std::vector<std::string> category_duty_text = {
        "Duty"
    };

    std::vector<QLineEdit*> line_edits;

public:
    MotorModuleGUI(uint8_t _module_num);
    void Deserialize(uint8_t* data, int size);
    void WidgetUpdate();
    QGroupBox *GetGroupBox();
};

}