#include <EncoderModuleGUI.h>
#include <HogeHoge.h>

using namespace HogeGen2;

EncoderModuleGUI::EncoderModuleGUI(uint8_t _module_num) : EncoderModule(_module_num) {

}

void EncoderModuleGUI::Deserialize(uint8_t* data, int size) {
    SetArrays(
        std::tuple<void*, void*, int>{pulse_array, &data[2], sizeof(short) * 4},
        std::tuple<void*, void*, int>{pose_array, &data[10], sizeof(float) * 5}
    );
}