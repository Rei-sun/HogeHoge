#include <SensorModuleGUI.h>
#include <IPCommunicationSub.h>
#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>

using namespace HogeGen2;

SensorModuleGUI::SensorModuleGUI(uint8_t _module_num) : SensorModule(_module_num) {
    IPCommunicationSub::RegisterModuleHoge(module_id, module_num, this);
}

void SensorModuleGUI::Deserialize(uint8_t* data, int size) {
    uint8_t *ba = (uint8_t*)&data[2];
    short *sa = (short*)&data[3];
    SetArrays(
        std::tuple<void*, void*, int>{digital_array, &data[2], sizeof(uint8_t) * 1},
        std::tuple<void*, void*, int>{analog_array, &data[3], sizeof(short) * 6}
    );
}

QGroupBox *SensorModuleGUI::GetGroupBox() {
    auto layout_vbox_ex = new QVBoxLayout();
    layout_vbox_ex->setSpacing(5);
    layout_vbox_ex->setContentsMargins(5,0,5,0);

    // レイアウト設定
    auto layout_hbox = new QHBoxLayout;
    layout_hbox->setSpacing(5);
    layout_hbox->setContentsMargins(5,0,5,0);
    
    // ラベル
    auto text = category_digital_text[0] + " " + std::to_string(module_num);
    auto label = new QLabel(text.c_str());
    label->setFixedSize(100, 15);
    layout_hbox->addWidget(label);

    for (int i = 0; i < 6; i++) {
        // 数値ラベル
        auto value_label = new QLabel(std::to_string(i+1).c_str());
        value_label->setAlignment(Qt::AlignRight);
        layout_hbox->addWidget(value_label);

        // 数値
        auto line_edit = new QLineEdit();
        line_edit->setReadOnly(true);
        line_edit->setFixedSize(20,18);
        line_edit->setText("0");
        line_edit->setAlignment(Qt::AlignRight);
        layout_hbox->addWidget(line_edit);
    }

    // レイアウト設定Ex
    auto layout_hbox_ex = new QHBoxLayout;
    layout_hbox_ex->setSpacing(5);
    layout_hbox_ex->setContentsMargins(5,0,5,0);
    
    // ラベルEx
    auto text_ex = category_analog_text[0] + " " + std::to_string(module_num);
    auto label_ex = new QLabel(text_ex.c_str());
    label_ex->setFixedSize(100, 15);
    layout_hbox_ex->addWidget(label_ex);

    for (int i = 0; i < 6; i++) {
        // 数値ラベルEx
        auto value_label = new QLabel(std::to_string(i+1).c_str());
        value_label->setAlignment(Qt::AlignCenter);
        layout_hbox_ex->addWidget(value_label);

        // 数値Ex
        auto line_edit = new QLineEdit();
        line_edit->setReadOnly(true);
        line_edit->setFixedSize(50,18);
        line_edit->setText("9999");
        line_edit->setAlignment(Qt::AlignRight);
        layout_hbox_ex->addWidget(line_edit);
    }

    layout_vbox_ex->addLayout(layout_hbox);
    layout_vbox_ex->addLayout(layout_hbox_ex);

    auto group = new QGroupBox();
    group->setLayout(layout_vbox_ex);

    return group;
}