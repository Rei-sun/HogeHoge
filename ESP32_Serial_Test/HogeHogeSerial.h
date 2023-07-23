#ifndef _H_HOGEHOGESERIAL_
#define _H_HOGEHOGESERIAL_

#include "CommunicationDefinition.h"
#include "ModuleDefinition.h"
#include "EncoderMod.h"
#include "SensorMod.h"
#include "HardwareSerial.h"
#include <vector>
#include <stdint.h>

using namespace HogeHoge;

class HogeHogeSerial {
  std::vector<uint8_t> buffer;
  EncoderMod &encoderMod;
  SensorMod &sensorMod;
public:
  HogeHogeSerial(EncoderMod &_encoderMod, SensorMod &_sensorMod) :
    buffer(),
    encoderMod(_encoderMod),
    sensorMod(_sensorMod)
  {
  
  }

  void ReceiveByte(uint8_t data) {
    buffer.push_back(data);

    if (buffer.size() <= 5) return;

    int data_length = 5 + buffer[4] + 1;

    if (buffer.size() != data_length) return;

    uint8_t module_id = buffer[0];
    uint8_t command = buffer[1];
    uint8_t module_num = buffer[2];
    uint8_t device_id = buffer[3];
    uint8_t length = buffer[4];
    uint8_t rx_data[255] = { 0 };
    for (int i = 0; i < buffer.size() - 6; i++) {
      rx_data[i] = buffer[5 + i];
    }
    uint8_t cs = buffer[buffer.size() - 1];

    switch (module_id) {
      case (uint8_t)ModuleID::MotorControlModule:
        // nothing
        break;
      case (uint8_t)ModuleID::EncoderModule:
        //if (module_num == 1) ;
        ResponseEncoder(command, module_num, device_id, length, rx_data);
        break;
      case (uint8_t)ModuleID::SensorModule:
        //if (module_num == 1) ;
        ResponseSensor(command, module_num, device_id, length, rx_data);
        break;
      case (uint8_t)ModuleID::SolenoidModule:
        // nothing
        break;
      default:
        break;
    }

    buffer.clear();
  }

  void Response(uint8_t mod_id, uint8_t cmd, uint8_t mod_n, uint8_t div_n, uint8_t length, void* data) {
    uint8_t tx_size = 5 + length + 1;
    uint8_t tx_data[255 + 5 + 1] = { 0 };
    tx_data[0] = mod_id;
    tx_data[1] = cmd;
    tx_data[2] = mod_n;
    tx_data[3] = div_n;
    tx_data[4] = length;
    for (int i = 0; i < length; i++) {
      tx_data[5 + i] = ((uint8_t*)data)[i];
    }
    tx_data[tx_size - 1] = CheckSum(tx_data, tx_size - 1);
    Serial.write(tx_data, tx_size);
  }

  void ResponseEncoder(uint8_t cmd, uint8_t mod_n, uint8_t div_n, uint8_t length, void* data) {
    if (cmd == (uint8_t)CMD_EncoderModule::GetLocalization) {
      float value_array[] = { encoderMod.GetPositionX(), encoderMod.GetPositionY(), encoderMod.GetEulerYaw() };
      Response((uint8_t)ModuleID::EncoderModule, cmd, mod_n, div_n, sizeof(float) * 3, value_array);
    } else if (cmd == (uint8_t)CMD_EncoderModule::GetAllPulse) {
      short value_array[] = { encoderMod.GetPulse(1), encoderMod.GetPulse(2), encoderMod.GetPulse(3), encoderMod.GetPulse(4) };
      Response((uint8_t)ModuleID::EncoderModule, cmd, mod_n, div_n, sizeof(float) * 4, value_array);
    } else {
      Response((uint8_t)ModuleID::EncoderModule, (uint8_t)CMD_EncoderModule::Invalid, mod_n, div_n, 0, nullptr);
    }
  }

  void ResponseSensor(uint8_t cmd, uint8_t mod_n, uint8_t div_n, uint8_t length, void* data) {
    if (cmd == (uint8_t)CMD_SensorModule::GetSensorData) {
      uint8_t byte_array[13] = {0};
      short value_array[] = { sensorMod.GetAnalog(1), sensorMod.GetAnalog(2), sensorMod.GetAnalog(3), sensorMod.GetAnalog(4), sensorMod.GetAnalog(5), sensorMod.GetAnalog(6) };
      byte_array[0] = sensorMod.switch_state.all;
      for (int i = 0; i < 12; i++) {
        byte_array[1 + i] = ((uint8_t*)value_array)[i];
      }
      Response((uint8_t)ModuleID::SensorModule, cmd, mod_n, div_n, sizeof(uint8_t) + sizeof(short) * 6, byte_array);
    } else {
      Response((uint8_t)ModuleID::SensorModule, (uint8_t)CMD_SensorModule::Invalid, mod_n, div_n, 0, nullptr);
    }
  }

  uint8_t CheckSum(uint8_t *data, size_t size) {
    uint8_t checksum = 0;
    for (int i = 0; i < size - 1; i++) {
      checksum += data[i];
    }
    return checksum;
  }
};

#endif