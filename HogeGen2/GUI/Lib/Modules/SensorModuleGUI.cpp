#include <SensorModuleGUI.h>
#include <HogeHoge.h>

using namespace HogeGen2;

SensorModuleGUI::SensorModuleGUI(uint8_t _module_num) : SensorModule(_module_num) {

}

void SensorModuleGUI::Deserialize(uint8_t* data, int size) {
    uint8_t *ba = (uint8_t*)&data[2];
    short *sa = (short*)&data[3];
    SetArrays(
        std::tuple<void*, void*, int>{digital_array, &data[2], sizeof(uint8_t) * 1},
        std::tuple<void*, void*, int>{analog_array, &data[3], sizeof(short) * 6}
    );
}