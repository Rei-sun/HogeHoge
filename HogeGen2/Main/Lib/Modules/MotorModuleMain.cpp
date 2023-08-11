#include <MotorModuleMain.h>
#include <HogeHoge.h>

using namespace HogeGen2;

MotorModuleMain::MotorModuleMain(uint8_t _module_num) : MotorModule(_module_num) {
    Hoge::RegisterBatchSender([&](){ SendCommand((uint8_t)CMD_MotorModule::SetAllDuty); });
}

void MotorModuleMain::SendCommand(uint8_t cmd) {
    //printf("%s, %s, cmd = %d, num = %d\n", __FUNCTION__, "MotorModuleMain", cmd, module_num);
    if (!Hoge::serial.IsConnect()) return;
    Hoge::serial.Send((uint8_t)module_id, (uint8_t)CMD_MotorModule::SetAllDuty, module_num, 0, sizeof(float) * 6, GetDutyArray());
}