#include <SolenoidModuleGUI.h>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>

using namespace HogeGen2;

SolenoidModuleGUI::SolenoidModuleGUI(uint8_t _module_num, IPCommunicationSub *ip_communication) : SolenoidModule(_module_num), control_window(this, ip_communication) {
    IPCommunicationSub::RegisterModuleHoge(module_id, module_num, this);
}

void SolenoidModuleGUI::Deserialize(uint8_t* data, int size) {
    SetArrays(
        std::tuple<void*, void*, int>{state_array, &data[0], sizeof(uint8_t) * 1}
    );
}

void SolenoidModuleGUI::WidgetUpdate() {
    for (int i = 0; i < (int)line_edits.size(); i++) {
        line_edits[i]->setText(QString::fromStdString(GetState(i+1) ? "1" : "0"));
    }
}

QGroupBox *SolenoidModuleGUI::GetGroupBox() {
    // レイアウト設定
    auto layout_hbox = new QHBoxLayout;
    layout_hbox->setSpacing(5);
    layout_hbox->setContentsMargins(5,0,5,0);
    
    // ラベル
    auto text = category_solenoid_text[0] + " " + std::to_string(module_num);
    auto label = new QLabel(text.c_str());
    label->setFixedSize(100, 15);
    layout_hbox->addWidget(label);

    // 8 個分あるので、8 を指定
    auto count_value = 8;
    for (int i = 0; i < count_value; i++) {
        // 数値ラベル
        auto value_label = new QLabel(std::to_string(i+1).c_str());
        value_label->setAlignment(Qt::AlignCenter);
        layout_hbox->addWidget(value_label);

        // 数値
        auto line_edit = new QLineEdit();
        line_edit->setReadOnly(true);
        line_edit->setFixedSize(20,18);
        line_edit->setAlignment(Qt::AlignRight);
        line_edit->setText("0");
        layout_hbox->addWidget(line_edit);
        line_edits.push_back(line_edit);
    }

    // 操作用パネル呼び出しボタン
    auto control_button = new QPushButton("control");
    control_button->setFixedSize(50,20);
    QObject::connect(control_button, &QPushButton::clicked, [=]() {
           OnButtonClick();
    });
    layout_hbox->addWidget(control_button);
    
    auto group = new QGroupBox();
    group->setLayout(layout_hbox);
    
    return group;
}

void SolenoidModuleGUI::OnButtonClick() {
    control_window.show();
}