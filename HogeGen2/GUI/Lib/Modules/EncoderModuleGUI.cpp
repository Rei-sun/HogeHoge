#include <EncoderModuleGUI.h>
#include <IPCommunicationSub.h>
#include <QLabel>
#include <QHBoxLayout>

#include <iostream>

using namespace HogeGen2;

EncoderModuleGUI::EncoderModuleGUI(uint8_t _module_num) : EncoderModule(_module_num) {
    IPCommunicationSub::RegisterModuleHoge(module_id, module_num, this);
}

void EncoderModuleGUI::Deserialize(uint8_t* data, int size) {
    SetArrays(
        std::tuple<void*, void*, int>{pulse_array, &data[0], sizeof(short) * 4},
        std::tuple<void*, void*, int>{pose_array, &data[8], sizeof(float) * 5}
    );
    WidgetUpdate();
}

void EncoderModuleGUI::WidgetUpdate() {
    for (int i = 0; i < (int)line_edits.size(); i++) {
        line_edits[i]->setText(QString::fromStdString(std::to_string(GetPulse(i+1))));
    }
}

QGroupBox *EncoderModuleGUI::GetGroupBox() {
    // レイアウト設定
    auto layout_hbox = new QHBoxLayout;
    layout_hbox->setSpacing(5);
    layout_hbox->setContentsMargins(5,0,5,0);
    
    // ラベル
    auto text = category_pulse_text[0] + " " + std::to_string(module_num);
    auto label = new QLabel(text.c_str());
    label->setFixedSize(100, 15);
    layout_hbox->addWidget(label);

    auto count_value = sizeof(pulse_array) / sizeof(short);
    for (int i = 0; i < count_value; i++) {
        // 数値ラベル
        auto value_label = new QLabel(std::to_string(i+1).c_str());
        value_label->setAlignment(Qt::AlignCenter);
        layout_hbox->addWidget(value_label);

        // 数値
        auto line_edit = new QLineEdit();
        line_edit->setReadOnly(true);
        line_edit->setFixedSize(60,18);
        line_edit->setText("-99999");
        line_edit->setAlignment(Qt::AlignRight);
        layout_hbox->addWidget(line_edit);
        line_edits.push_back(line_edit);
    }

    auto group = new QGroupBox();
    group->setLayout(layout_hbox);
    return group;
}