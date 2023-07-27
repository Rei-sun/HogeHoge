#include <SolenoidModule.h>

using namespace HogeHoge;

bool SolenoidModule::Command(uint8_t cmd, uint8_t device_id, uint8_t length, void* data) {
    if(!serial.IsConnect()) return false;
    serial.Send((uint8_t)module_id, cmd, module_num, device_id, length, data);
    return true;
}

SolenoidModule::SolenoidModule(HogeHogeSerial &_serial, uint8_t _module_num):
    BaseModule(_serial, ModuleID::SolenoidModule, _module_num),
    in_solenoid_state{0}
{

}

void SolenoidModule::SendBatch() {
    Command((uint8_t)CMD_SolenoidModule::SetAllState, 0, sizeof(uint8_t), &in_solenoid_state.all);
}

std::pair<uint8_t, std::shared_ptr<uint8_t[]>> SolenoidModule::Serialized() {
    uint8_t byte_array[] {
        in_solenoid_state.all
    };

    uint8_t ba_size = sizeof(byte_array);

    if (serialized.get() == nullptr) {
        serialized = std::shared_ptr<uint8_t[]>(new uint8_t[2 + ba_size]);
    }

    serialized.get()[0] = (uint8_t)module_id;
    serialized.get()[1] = (uint8_t)module_num;
    memcpy(serialized.get() + 2, byte_array, ba_size);

    return std::make_pair(2 + ba_size, serialized);
}

void SolenoidModule::SetState(uint8_t device_id, bool state) {
    // Argument check
    if (device_id > 8 || device_id < 1) return;

    if (state) {
        in_solenoid_state.all |= (1 << (device_id - 1));
    } else {
        in_solenoid_state.all &= ~(1 << (device_id - 1));
    }
}

bool SolenoidModule::GetState(uint8_t device_id) {
    // Argument check
    if (device_id > 8 || device_id < 1) return false;

    return in_solenoid_state.all >> (device_id - 1) & 1;
}