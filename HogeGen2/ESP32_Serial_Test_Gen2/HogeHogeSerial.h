#ifndef _H_HOGEHOGESERIAL_
#define _H_HOGEHOGESERIAL_

#include "CommandDefinition.h"
#include "Module.h"
#include "ModuleManager.h"
#include "HardwareSerial.h"
#include <vector>
#include <stdint.h>

using namespace HogeGen2;

class HogeHogeSerial {
  std::vector<uint8_t> buffer;
public:
  HogeHogeSerial() :
    buffer()
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
      case (uint8_t)ModuleID::MotorModule:
        ReceiveMotor(command, module_num, device_id, length, rx_data);
        break;
      case (uint8_t)ModuleID::EncoderModule:
        ResponseEncoder(command, module_num, device_id, length, rx_data);
        break;
      case (uint8_t)ModuleID::SensorModule:
        ResponseSensor(command, module_num, device_id, length, rx_data);
        break;
      case (uint8_t)ModuleID::SolenoidModule:
        ReceiveSolenoid(command, module_num, device_id, length, rx_data);
        break;
      default:
        break;
    }

    buffer.clear();
  }

  void Response(uint8_t mod_id, uint8_t cmd, uint8_t mod_n, uint8_t dev_n, uint8_t length, void* data) {
    uint8_t tx_size = 5 + length + 1;
    uint8_t tx_data[255 + 5 + 1] = { 0 };
    tx_data[0] = mod_id;
    tx_data[1] = cmd;
    tx_data[2] = mod_n;
    tx_data[3] = dev_n;
    tx_data[4] = length;
    for (int i = 0; i < length; i++) {
      tx_data[5 + i] = ((uint8_t*)data)[i];
    }
    tx_data[tx_size - 1] = CheckSum(tx_data, tx_size - 1);
    Serial.write(tx_data, tx_size);
  }

  void ResponseEncoder(uint8_t cmd, uint8_t mod_n, uint8_t dev_n, uint8_t length, void* data) {
    // モジュールナンバーチェック
    if (ModuleManager::GetEncoderModules().size() < mod_n) {
      Response((uint8_t)ModuleID::EncoderModule, (uint8_t)CMD_EncoderModule::Invalid, mod_n, dev_n, 0, nullptr);
    }
    
    // コマンド処理
    if (cmd == (uint8_t)CMD_EncoderModule::GetLocalization) {
      // 場所、姿勢は module_id = 1 限定、それ以外は Invalid を返す。
      if (mod_n == 1) {
        float value_array[] = { ModuleManager::GetEncoderModules()[0]->GetPose(1), ModuleManager::GetEncoderModules()[0]->GetPose(2), ModuleManager::GetEncoderModules()[0]->GetPose(3), ModuleManager::GetEncoderModules()[0]->GetPose(4), ModuleManager::GetEncoderModules()[0]->GetPose(5) };
        Response((uint8_t)ModuleID::EncoderModule, cmd, mod_n, dev_n, sizeof(float) * 5, value_array);
      } else {
        Response((uint8_t)ModuleID::EncoderModule, (uint8_t)CMD_EncoderModule::Invalid, mod_n, dev_n, 0, nullptr);  
      }
    } else if (cmd == (uint8_t)CMD_EncoderModule::GetAllPulse) {
      short value_array[] = { ModuleManager::GetEncoderModules()[mod_n-1]->GetPulse(1), ModuleManager::GetEncoderModules()[mod_n-1]->GetPulse(2), ModuleManager::GetEncoderModules()[mod_n-1]->GetPulse(3), ModuleManager::GetEncoderModules()[mod_n-1]->GetPulse(4) };
      Response((uint8_t)ModuleID::EncoderModule, cmd, mod_n, dev_n, sizeof(short) * 4, value_array);
    } else {
      Response((uint8_t)ModuleID::EncoderModule, (uint8_t)CMD_EncoderModule::Invalid, mod_n, dev_n, 0, nullptr);
    }
  }

  void ResponseSensor(uint8_t cmd, uint8_t mod_n, uint8_t dev_n, uint8_t length, void* data) {
    // モジュールナンバーチェック
    if (ModuleManager::GetSensorModules().size() < mod_n) {
      Response((uint8_t)ModuleID::SensorModule, (uint8_t)CMD_SensorModule::Invalid, mod_n, dev_n, 0, nullptr);
    }
    
    // コマンド処理
    if (cmd == (uint8_t)CMD_SensorModule::GetSensorData) {
      uint8_t byte_array[13] = {0};
      short value_array[] = { 
        ModuleManager::GetSensorModules()[mod_n-1]->GetAnalog(1),
        ModuleManager::GetSensorModules()[mod_n-1]->GetAnalog(2),
        ModuleManager::GetSensorModules()[mod_n-1]->GetAnalog(3),
        ModuleManager::GetSensorModules()[mod_n-1]->GetAnalog(4),
        ModuleManager::GetSensorModules()[mod_n-1]->GetAnalog(5),
        ModuleManager::GetSensorModules()[mod_n-1]->GetAnalog(6)
      };
      byte_array[0] = ModuleManager::GetSensorModules()[mod_n-1]->GetDigitalArray()[0].all;
      for (int i = 0; i < 12; i++) {
        byte_array[1 + i] = ((uint8_t*)value_array)[i];
      }
      Response((uint8_t)ModuleID::SensorModule, cmd, mod_n, dev_n, sizeof(uint8_t) + sizeof(short) * 6, byte_array);
    } else {
      Response((uint8_t)ModuleID::SensorModule, (uint8_t)CMD_SensorModule::Invalid, mod_n, dev_n, 0, nullptr);
    }
  }

  void ReceiveMotor(uint8_t cmd, uint8_t mod_n, uint8_t dev_n, uint8_t length, void* data) {
    if (cmd == (uint8_t)CMD_MotorModule::SetAllDuty) {
      float *duty_array = (float*)data;
      for(int i = 0; i < 6; i++)  ModuleManager::GetMotorModules()[mod_n-1]->SetDuty(i + 1, duty_array[i]);
    }
  }

  void ReceiveSolenoid(uint8_t cmd, uint8_t mod_n, uint8_t dev_n, uint8_t length, void* data) {
    if (cmd == (uint8_t)CMD_SolenoidModule::SetAllState) {
      uint8_t *data_array = (uint8_t*)data;
      ModuleManager::GetSolenoidModules()[mod_n-1]->GetStateArray()[0].all = data_array[0];
    }
  }
};

#endif