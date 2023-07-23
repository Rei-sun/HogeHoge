#include <SensorModule.h>

using namespace HogeHoge;

SensorModule::SensorModule(HogeHogeSerial &_serial, uint8_t _module_num):
    BaseModule(_serial, ModuleID::SensorModule, _module_num),
    in_swich_state{0},
    in_analog_1(0),
    in_analog_2(0),
    in_analog_3(0),
    in_analog_4(0),
    in_analog_5(0),
    in_analog_6(0),
    value_map{
        &in_analog_1,
        &in_analog_2,
        &in_analog_3,
        &in_analog_4,
        &in_analog_5,
        &in_analog_6
    }
{

}

bool SensorModule::Command(uint8_t cmd, uint8_t device_id, uint8_t length, void* data) {
    if(!serial.IsConnect()) return false;
    
    wait_for_response = true;
    
    serial.Send((uint8_t)module_id, cmd, module_num, device_id, length, data);
    
    while (wait_for_response) {
        continue;
    }

    return true;
}

void SensorModule::Receive(uint8_t cmd, uint8_t device_id, uint8_t length, void* data) {
    wait_for_response = false;
    
    if (cmd == (uint8_t)CMD_SensorModule::GetSensorData) {
        auto uint8_data = (uint8_t*)data;
        in_swich_state.all = uint8_data[0];
        auto short_array = (short*)((uint8_t*)data + 1);
        for (int i = 0; i < 6; i++) {
            *value_map[i] = short_array[i];
        }
        printf("0x%02x, %5d, %5d, %5d, %5d, %5d, %5d\n", in_swich_state.all, in_analog_1, in_analog_2, in_analog_3, in_analog_4, in_analog_5, in_analog_6);
    } else {
        printf("undefined command\n");
    }
}

void SensorModule::SendGetSensorData() {
    Command((uint8_t)CMD_SensorModule::GetSensorData, 0, 0, nullptr);
}

bool SensorModule::GetSwitch(uint8_t device_id) {
    // Argument check
    if (device_id > 7 || device_id < 1) return false;

    return in_swich_state.all >> (device_id - 1) & 1;
}

short SensorModule::GetAnalog(uint8_t device_id) {
    // Argument check
    if (device_id > 6 || device_id < 1) return false;

    return *value_map[device_id - 1];
}