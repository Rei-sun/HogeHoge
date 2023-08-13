#include <MotorModuleMain.h>
#include <HogeHoge.h>

using namespace HogeGen2;

MotorModuleMain::MotorModuleMain(uint8_t _module_num) : MotorModule(_module_num) {
    Hoge::RegisterBatchSender([&](){ SendCommand((uint8_t)CMD_MotorModule::SetAllDuty); });
    Hoge::RegisterIPSerialize(module_name, static_cast<IModuleSerializer*>(this));
    Hoge::RegisterIPControl(module_name, static_cast<IModuleController*>(this));
}

void MotorModuleMain::SendCommand(uint8_t cmd) {
    //printf("%s, %s, cmd = %d, num = %d\n", __FUNCTION__, "MotorModuleMain", cmd, module_num);
    if (!Hoge::serial.IsConnect()) return;
    Hoge::serial.Send((uint8_t)module_id, (uint8_t)CMD_MotorModule::SetAllDuty, module_num, 0, sizeof(float) * 6, GetDutyArray());
}

std::pair<uint8_t, std::shared_ptr<uint8_t[]>> MotorModuleMain::Serialized() {
    uint8_t fa_size = sizeof(duty_array);

    if (serialized.get() == nullptr) {
        serialized = std::shared_ptr<uint8_t[]>(new uint8_t[2 + fa_size]);
    }

    serialized.get()[0] = (uint8_t)module_id;
    serialized.get()[1] = (uint8_t)module_num;
    memcpy(serialized.get() + 2, duty_array, fa_size);

    return std::make_pair(2 + fa_size, serialized);
}

void MotorModuleMain::Control(uint8_t id, float value) {
    SetDuty(id, value);
}