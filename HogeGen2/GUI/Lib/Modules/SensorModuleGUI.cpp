#include <SensorModuleGUI.h>
#include <IPCommunicationSub.h>
#include <QLabel>
#include <QHBoxLayout>

using namespace HogeGen2;

SensorModuleGUI::SensorModuleGUI(uint8_t _module_num, IPCommunicationSub *ip_communication) : SensorModule(_module_num) {
    IPCommunicationSub::RegisterModuleHoge(module_id, module_num, this);
}

void SensorModuleGUI::Deserialize(uint8_t* data, int size) {
    SetArrays(
        std::tuple<void*, void*, int>{digital_array, &data[0], sizeof(uint8_t) * 1},
        std::tuple<void*, void*, int>{analog_array, &data[1], sizeof(short) * 6}
    );
    WidgetUpdate();
}

void SensorModuleGUI::WidgetUpdate() {
    for (int i = 0; i < (int)line_edits.size(); i++) {
        line_edits[i]->setText(QString::fromStdString(std::to_string(GetDigital(i+1))));
    }
    for (int i = 0; i < (int)line_edits_ex.size(); i++) {
        line_edits_ex[i]->setText(QString::fromStdString(std::to_string(GetAnalog(i+1))));
    }
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

    // 6 個分あるので、6 を指定
    auto count_value = 6;
    for (int i = 0; i < count_value; i++) {
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
        line_edits.push_back(line_edit);
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

    auto count_value_ex = sizeof(analog_array) / sizeof(short);
    for (int i = 0; i < count_value_ex; i++) {
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
        line_edits_ex.push_back(line_edit);
    }

    layout_vbox_ex->addLayout(layout_hbox);
    layout_vbox_ex->addLayout(layout_hbox_ex);

    auto group = new QGroupBox();
    group->setLayout(layout_vbox_ex);

    return group;
}