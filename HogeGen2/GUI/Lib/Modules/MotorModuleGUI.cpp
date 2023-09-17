#include <MotorModuleGUI.h>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QObject>
#include <sstream>
#include <iomanip>

using namespace HogeGen2;

MotorModuleGUI::MotorModuleGUI(uint8_t _module_num, IPCommunicationSub *ip_communication) : MotorModule(_module_num), control_window(this, ip_communication) {
    IPCommunicationSub::RegisterModuleHoge(module_id, module_num, this);
}

void MotorModuleGUI::Deserialize(uint8_t* data, int size) {
    SetArrays(
        std::tuple<void*, void*, int>{duty_array, &data[0], sizeof(float) * 6}
    );
    WidgetUpdate();
}

void MotorModuleGUI::WidgetUpdate() {
    for (int i = 0; i < (int)line_edits.size(); i++) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << GetDuty(i+1);
        line_edits[i]->setText(QString::fromStdString(ss.str()));
    }
}

QGroupBox *MotorModuleGUI::GetGroupBox() {
    // レイアウト設定
    auto layout_hbox = new QHBoxLayout;
    layout_hbox->setSpacing(5);
    layout_hbox->setContentsMargins(5,0,5,0);
    
    // ラベル
    auto text = category_duty_text[0] + " " + std::to_string(module_num);
    auto label = new QLabel(text.c_str());
    label->setFixedSize(100, 15);
    layout_hbox->addWidget(label);

    auto count_value = sizeof(duty_array) / sizeof(float);
    for (int i = 0; i < count_value; i++) {
        // 数値ラベル
        auto value_label = new QLabel(std::to_string(i+1).c_str());
        value_label->setAlignment(Qt::AlignCenter);
        layout_hbox->addWidget(value_label);

        // 数値
        auto line_edit = new QLineEdit();
        line_edit->setReadOnly(true);
        line_edit->setFixedSize(60,18);
        line_edit->setText("100.00");
        line_edit->setAlignment(Qt::AlignRight);
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

    // 最後にStretchをいれて余白を詰める。
    layout_hbox->addStretch();

    auto group = new QGroupBox();
    group->setLayout(layout_hbox);
    
    return group;
}

void MotorModuleGUI::OnButtonClick() {
    control_window.show();
}