#include <SensorModuleMain.h>
#include <HogeHoge.h>

using namespace HogeGen2;

SensorModuleMain::SensorModuleMain(uint8_t _module_num) : SensorModule(_module_num) {
    Hoge::RegisterRequestSensor([&](){ SendCommand((uint8_t)CMD_SensorModule::GetSensorData); });
}

void SensorModuleMain::ReceiveCommand(uint8_t cmd, uint8_t dev_id, uint8_t length, void *data) {
    // WaitForResponse Flag Set
    SetWaitForResponse(false);

    // Conditional split for each command.
    if (cmd == (uint8_t)CMD_SensorModule::GetSensorData) {
        auto uint8_data = (uint8_t*)data;
        auto short_array = (short*)((uint8_t*)data + 1);
        GetDigitalArray()[0].all = uint8_data[0];
        for (int i = 0; i < 4; i++) {
            SetAnalog(i + 1, short_array[i]);
        }
    } else {
        // printf("undefined command\n");
    }
}

void SensorModuleMain::SendCommand(uint8_t cmd) {
    //printf("%s, %s, cmd = %d, num = %d\n", __FUNCTION__, "SensorModuleMain", cmd, module_num);
    if (!Hoge::serial.IsConnect()) return;
    SetWaitForResponse(true);
    Hoge::serial.Send((uint8_t)module_id, cmd, module_num, 0, 0, nullptr);
    while (GetWaitForResponse() && Hoge::Good()) {}
    SetWaitForResponse(false);
}