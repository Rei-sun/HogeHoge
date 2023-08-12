#include <MotorModuleGUI.h>
#include <HogeHoge.h>

using namespace HogeGen2;

MotorModuleGUI::MotorModuleGUI(uint8_t _module_num) : MotorModule(_module_num) {

}

void MotorModuleGUI::Deserialize(uint8_t* data, int size) {
    float *fa = (float*)&data[2];
    SetArrays(
        std::tuple<void*, void*, int>{duty_array, &data[2], sizeof(float) * 6}
    );
}