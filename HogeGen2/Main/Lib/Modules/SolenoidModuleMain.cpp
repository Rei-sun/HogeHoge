#include <SolenoidModuleMain.h>
#include <HogeHoge.h>

using namespace HogeGen2;

SolenoidModuleMain::SolenoidModuleMain(uint8_t _module_num) : SolenoidModule(_module_num) {
    Hoge::RegisterBatchSender([&](){ SendCommand((uint8_t)CMD_SolenoidModule::SetAllState); });
    Hoge::RegisterIPSerialize(module_name, static_cast<IModuleSerializer*>(this));
    Hoge::RegisterIPControl(module_name, static_cast<IModuleController*>(this));
}

void SolenoidModuleMain::SendCommand(uint8_t cmd) {
    //printf("%s, %s, cmd = %d, num = %d\n", __FUNCTION__, "SolenoidModuleMain", cmd, module_num);
    if (!Hoge::serial.IsConnect()) return;
    Hoge::serial.Send((uint8_t)module_id, cmd, module_num, 0, sizeof(uint8_t) * 1, &GetStateArray()->all);
}

std::pair<uint8_t, std::shared_ptr<uint8_t[]>> SolenoidModuleMain::Serialized() {
    uint8_t ba_size = sizeof(state_array);

    if (serialized.get() == nullptr) {
        serialized = std::shared_ptr<uint8_t[]>(new uint8_t[3 + ba_size]);
    }

    serialized.get()[0] = (uint8_t)module_id;
    serialized.get()[1] = (uint8_t)module_num;
    serialized.get()[2] = (uint8_t)ba_size;
    memcpy(serialized.get() + 3, state_array, ba_size);

    return std::make_pair(3 + ba_size, serialized);
}

void SolenoidModuleMain::Control(uint8_t id, float value) {
    SetState(id, value == 0 ? false : true);
}