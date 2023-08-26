#include <MotorModuleGUI.h>

#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

using namespace HogeGen2;

MotorModuleGUI::MotorModuleGUI(uint8_t _module_num) : MotorModule(_module_num) {

}

void MotorModuleGUI::Deserialize(uint8_t* data, int size) {
    float *fa = (float*)&data[2];
    SetArrays(
        std::tuple<void*, void*, int>{duty_array, &data[2], sizeof(float) * 6}
    );
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

    for (int i = 0; i < 6; i++) {
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
    }

    // 操作用パネル呼び出しボタン
    auto control_button = new QPushButton("control");
    control_button->setFixedSize(50,20);
    layout_hbox->addWidget(control_button);

    auto group = new QGroupBox();
    group->setLayout(layout_hbox);
    
    return group;
}