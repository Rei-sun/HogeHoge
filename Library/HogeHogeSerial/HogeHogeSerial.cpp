#include <HogeHogeSerial.h>

using namespace HogeHoge;

void HogeHogeSerial::OnReceive(void* recv_data, size_t recv_size) {
    auto data = (uint8_t*)recv_data;
    
    printf("recv %ld byte: ", recv_size);

    for (int i = 0; i < recv_size; i++) {
        printf("%x, ", data[i]);   
    }
    printf("\n");
}

HogeHogeSerial::HogeHogeSerial(std::string device_name) :
    SerialCommunication(),
    target_device_name(device_name)
{
    RegisterCallbackOnReceive([this](void* data, size_t size){ OnReceive(data, size); });
    Open(target_device_name, true);
}

void HogeHogeSerial::Send(uint8_t module_id, uint8_t commad, uint8_t module_num, uint8_t device_id, uint8_t length, void *value_data) {
    // argument check
    
    // new tx_data (size = 5 + length + checksum)
    uint8_t tx_size = 5 + length + 1;
    uint8_t *tx_data = new uint8_t[tx_size];

    // To constructing transmit data
    tx_data[0] = module_id;
    tx_data[1] = commad;
    tx_data[2] = module_num;
    tx_data[3] = device_id;
    tx_data[4] = length;

    uint8_t *uvalue_data = (uint8_t*)value_data;
    for (int i = 0; i < length; i++) {
        tx_data[5 + i] = uvalue_data[i];
    }

    // Add checksum
    uint8_t checksum = 0;
    for (int i = 0; i < tx_size - 1; i++) {
        printf("%x:", tx_data[i]);
        checksum += tx_data[i];
    }
    tx_data[tx_size - 1] = checksum;

    // Transmit
    Transmit(tx_data, tx_size);

    // delete   
    delete[] tx_data;
}