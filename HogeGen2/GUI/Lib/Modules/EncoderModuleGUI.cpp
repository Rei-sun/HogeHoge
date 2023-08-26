#include <EncoderModuleGUI.h>
#include <IPCommunicationSub.h>
#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>

using namespace HogeGen2;

EncoderModuleGUI::EncoderModuleGUI(uint8_t _module_num) : EncoderModule(_module_num) {
    IPCommunicationSub::RegisterModuleHoge(module_id, module_num, this);
}

void EncoderModuleGUI::Deserialize(uint8_t* data, int size) {
    SetArrays(
        std::tuple<void*, void*, int>{pulse_array, &data[2], sizeof(short) * 4},
        std::tuple<void*, void*, int>{pose_array, &data[10], sizeof(float) * 5}
    );
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

    for (int i = 0; i < 4; i++) {
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
    }

    auto group = new QGroupBox();
    group->setLayout(layout_hbox);
    return group;
}