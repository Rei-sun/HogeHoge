#pragma once

#include <Module.h>
#include <IModuleDeserializer.h>
#include <IGroupBox.h>
#include <QLineEdit>
#include <SubWindow.h>
#include <QSlider>
#include <IPCommunicationSub.h>

namespace HogeGen2 {

class MotorModuleGUI : public MotorModule, public IModuleDeserializer, public IGroupBox {
    static inline std::vector<std::string> category_duty_text = {
        "Duty"
    };

    std::vector<QLineEdit*> line_edits;

    class MotorModuleWindow : public SubWindow {
        MotorModuleGUI *module_reference;
        IPCommunicationSub *ip_communication;
    public:
        MotorModuleWindow(MotorModuleGUI *module, IPCommunicationSub *ip):
            SubWindow(),
            module_reference(module),
            ip_communication(ip)
        {
            auto controls = new QVBoxLayout(this);
            for (int i = 0; i < 6; i++) {
                auto control = new QHBoxLayout();
                auto label = new QLabel(QString::fromStdString(std::to_string(i + 1)));
                control->addWidget(label);
                auto slider = new QSlider();
                slider->setOrientation(Qt::Horizontal);
                slider->setMinimum(0);
                slider->setMaximum(10000);
                QObject::connect(slider, &QSlider::valueChanged, [=]() {
                    float value = slider->value() / 100.0f;
                    ip_communication->SendFoo(*module, i + 1, value);
                });
                control->addWidget(slider);
                controls->addLayout(control);
            }
        }
    };
    MotorModuleWindow control_window;

public:
    MotorModuleGUI(uint8_t _module_num, IPCommunicationSub *ip_communication);
    void Deserialize(uint8_t* data, int size);
    void WidgetUpdate();
    QGroupBox *GetGroupBox();
    void OnButtonClick();
};

}