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
    while (GetWaitForResponse() && Hoge::Good() && Hoge::serial.IsConnect()) {}
    SetWaitForResponse(false);
}

std::pair<uint8_t, std::shared_ptr<uint8_t[]>> SensorModuleMain::Serialized() {
    uint8_t ba_size = sizeof(digital_array);
    uint8_t sa_size = sizeof(analog_array);

    if (serialized.get() == nullptr) {
        serialized = std::shared_ptr<uint8_t[]>(new uint8_t[2 + ba_size + sa_size]);
    }

    serialized.get()[0] = (uint8_t)module_id;
    serialized.get()[1] = (uint8_t)module_num;
    memcpy(serialized.get() + 2, digital_array, ba_size);
    memcpy(serialized.get() + 2 + ba_size, analog_array, sa_size);

    return std::make_pair(2 + ba_size + sa_size, serialized);
}