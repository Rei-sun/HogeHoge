#pragma once

#include <IPCommunicationSub.h>
#include <Module.h>
#include <IModuleDeserializer.h>
#include <IGroupBox.h>
#include <QLineEdit>
#include <SubWindow.h>
#include <QCheckBox>

namespace HogeGen2 {

class SolenoidModuleGUI : public SolenoidModule, public IModuleDeserializer, public IGroupBox {
    static inline std::vector<std::string> category_solenoid_text = {
        "Solenoid"
    };

    std::vector<QLineEdit*> line_edits;

    class SolenoidModuleWindow : public SubWindow {
        SolenoidModuleGUI *module_reference;
        IPCommunicationSub *ip_communication;
    public:
        SolenoidModuleWindow(SolenoidModuleGUI *module, IPCommunicationSub *ip):
            SubWindow(),
            module_reference(module),
            ip_communication(ip)
        {
            auto controls = new QVBoxLayout(this);
            for (int i = 0; i < 8; i++) {
                auto control = new QHBoxLayout();
                auto label = new QLabel(QString::fromStdString(std::to_string(i + 1)));
                control->addWidget(label);
                auto checkbox = new QCheckBox();
                QObject::connect(checkbox, &QCheckBox::stateChanged, [=]() {
                    float value = checkbox->checkState();
                    ip_communication->SendFoo(*module, i + 1, value);
                });
                control->addWidget(checkbox);
                controls->addLayout(control);
            }
        }
    };
    SolenoidModuleWindow control_window;

public:
    SolenoidModuleGUI(uint8_t _module_num, IPCommunicationSub *ip_communication);
    void Deserialize(uint8_t* data, int size);
    void WidgetUpdate();
    QGroupBox *GetGroupBox();
    void OnButtonClick();
};

}
