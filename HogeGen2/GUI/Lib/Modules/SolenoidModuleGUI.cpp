#include <SolenoidModuleGUI.h>
#include <HogeHoge.h>

using namespace HogeGen2;

SolenoidModuleGUI::SolenoidModuleGUI(uint8_t _module_num) : SolenoidModule(_module_num) {

}

void SolenoidModuleGUI::Deserialize(uint8_t* data, int size) {
    uint8_t *ba = (uint8_t*)&data[2];
    SetArrays(
        std::tuple<void*, void*, int>{state_array, &data[2], sizeof(uint8_t) * 1}
    );
}