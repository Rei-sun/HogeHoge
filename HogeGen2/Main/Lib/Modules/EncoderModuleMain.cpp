#include <EncoderModuleMain.h>
#include <HogeHoge.h>

using namespace HogeGen2;

EncoderModuleMain::EncoderModuleMain(uint8_t _module_num) : EncoderModule(_module_num) {
    if (module_num == 1) {
        Hoge::RegisterRequestSensor([&](){ SendCommand((uint8_t)CMD_EncoderModule::GetLocalization); });
    }
    Hoge::RegisterRequestSensor([&](){ SendCommand((uint8_t)CMD_EncoderModule::GetAllPulse); });
    Hoge::RegisterIPSerialize(module_name, this);
}

void EncoderModuleMain::ReceiveCommand(uint8_t cmd, uint8_t dev_id, uint8_t length, void *data) {
    // WaitForResponse Flag Set
    SetWaitForResponse(false);

    // Conditional split for each command.
    if (cmd == (uint8_t)CMD_EncoderModule::GetAllPulse) {
        auto pulse_array = (short*)data;
        for (int i = 0; i < 4; i++) {
            SetPulse(i + 1, pulse_array[i]);
        }
    } else if (cmd == (uint8_t)CMD_EncoderModule::GetLocalization) {
        auto pose_array = (float*)data;
        SetPose(1, pose_array[0]);
        SetPose(2, pose_array[1]);
        SetPose(3, pose_array[2]);
        SetPose(4, pose_array[3]);
        SetPose(5, pose_array[4]);
    } else {
        // printf("undefined command\n");
    }
}

void EncoderModuleMain::SendCommand(uint8_t cmd) {
    //printf("%s, %s, cmd = %d, num = %d\n", __FUNCTION__, "EncoderModuleMain", cmd, module_num);
    if (!Hoge::serial.IsConnect()) return;
    SetWaitForResponse(true);
    Hoge::serial.Send((uint8_t)module_id, cmd, module_num, 0, 0, nullptr);
    while (GetWaitForResponse() && Hoge::Good() && Hoge::serial.IsConnect()) {}
    SetWaitForResponse(false);
}

std::pair<uint8_t, std::shared_ptr<uint8_t[]>> EncoderModuleMain::Serialized() {
    uint8_t sa_size = sizeof(pulse_array);
    uint8_t fa_size = sizeof(pose_array);

    if (serialized.get() == nullptr) {
        serialized = std::shared_ptr<uint8_t[]>(new uint8_t[3 + sa_size + fa_size]);
    }

    serialized.get()[0] = (uint8_t)module_id;
    serialized.get()[1] = (uint8_t)module_num;
    serialized.get()[2] = (uint8_t)(sa_size + fa_size);
    memcpy(serialized.get() + 3, pulse_array, sa_size);
    memcpy(serialized.get() + 3 + sa_size, pose_array, fa_size);

    return std::make_pair(3 + sa_size + fa_size, serialized);
}