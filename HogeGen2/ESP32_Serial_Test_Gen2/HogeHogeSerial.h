#ifndef _H_HOGEHOGESERIAL_
#define _H_HOGEHOGESERIAL_

#include "CommandDefinition.h"
#include "Module.h"
#include "ModuleManager.h"
#include "OutputComponent.h"
#include "HardwareSerial.h"
#include <vector>
#include <stdint.h>

using namespace HogeGen2;

extern LEDOut led_out_3;

class HogeHogeSerial {
  uint16_t rx_idx;
  uint8_t rx_buffer[255 + 5 + 1];
  uint8_t tx_buffer[255 + 5 + 1];
public:
  HogeHogeSerial() :
    rx_idx(0),
    rx_buffer{0},
    tx_buffer{0}
  {}

  void ReceiveByte(uint8_t data) {
    led_out_3.Write(true);
    rx_buffer[rx_idx++] = data;

    if (rx_idx <= 5) return;

    int data_length = 5 + rx_buffer[4] + 1;

    if (rx_idx != data_length) return;

    uint8_t module_id = rx_buffer[0];
    uint8_t command = rx_buffer[1];
    uint8_t module_num = rx_buffer[2];
    uint8_t device_id = rx_buffer[3];
    uint8_t length = rx_buffer[4];
    uint8_t cs = rx_buffer[rx_idx - 1];

    switch (module_id) {
      case (uint8_t)ModuleID::MotorModule:
        ReceiveMotor(command, module_num, device_id, length, rx_buffer + 5);
        break;
      case (uint8_t)ModuleID::EncoderModule:
        ResponseEncoder(command, module_num, device_id, length, rx_buffer + 5);
        break;
      case (uint8_t)ModuleID::SensorModule:
        ResponseSensor(command, module_num, device_id, length, rx_buffer + 5);
        break;
      case (uint8_t)ModuleID::SolenoidModule:
        ReceiveSolenoid(command, module_num, device_id, length, rx_buffer + 5);
        break;
      default:
        break;
    }

    rx_idx = 0;
    led_out_3.Write(false);
  }

  void Response(uint8_t mod_id, uint8_t cmd, uint8_t mod_n, uint8_t dev_n, uint8_t length, void* data) {
    uint8_t tx_size = 5 + length + 1;
    tx_buffer[0] = mod_id;
    tx_buffer[1] = cmd;
    tx_buffer[2] = mod_n;
    tx_buffer[3] = dev_n;
    tx_buffer[4] = length;
    for (int i = 0; i < length; i++) {
      tx_buffer[5 + i] = ((uint8_t*)data)[i];
    }
    tx_buffer[tx_size - 1] = CheckSum(tx_buffer, tx_size - 1);
    Serial.write(tx_buffer, tx_size);
  }

  void ResponseEncoder(uint8_t cmd, uint8_t mod_n, uint8_t dev_n, uint8_t length, void* data) {
    // モジュールナンバーチェック
    if (ModuleManager::GetEncoderModules().size() < mod_n) {
      Response((uint8_t)ModuleID::EncoderModule, (uint8_t)CMD_EncoderModule::Invalid, mod_n, dev_n, 0, nullptr);
    }
    
    auto module = ModuleManager::GetEncoderModules()[mod_n - 1];

    // コマンド処理
    if (cmd == (uint8_t)CMD_EncoderModule::GetLocalization) {
      // 場所、姿勢は module_id = 1 限定、それ以外は Invalid を返す。
      if (mod_n == 1) {
        Response((uint8_t)ModuleID::EncoderModule, cmd, mod_n, dev_n, sizeof(float) * 5, module->GetPoseArray());
      } else {
        Response((uint8_t)ModuleID::EncoderModule, (uint8_t)CMD_EncoderModule::Invalid, mod_n, dev_n, 0, nullptr);  
      }
    } else if (cmd == (uint8_t)CMD_EncoderModule::GetAllPulse) {
      Response((uint8_t)ModuleID::EncoderModule, cmd, mod_n, dev_n, sizeof(short) * 4, module->GetPulseArray());
    } else {
      Response((uint8_t)ModuleID::EncoderModule, (uint8_t)CMD_EncoderModule::Invalid, mod_n, dev_n, 0, nullptr);
    }
  }

  void ResponseSensor(uint8_t cmd, uint8_t mod_n, uint8_t dev_n, uint8_t length, void* data) {
    static uint8_t byte_array[13] = {0};
    // モジュールナンバーチェック
    if (ModuleManager::GetSensorModules().size() < mod_n) {
      Response((uint8_t)ModuleID::SensorModule, (uint8_t)CMD_SensorModule::Invalid, mod_n, dev_n, 0, nullptr);
    }
    
    auto module = ModuleManager::GetSensorModules()[mod_n - 1];

    // コマンド処理
    if (cmd == (uint8_t)CMD_SensorModule::GetSensorData) {
      auto aa = module->GetAnalogArray();
      byte_array[0] = module->GetDigitalArray()[0].all;
      for (int i = 0; i < 12; i++) {
        byte_array[1 + i] = ((uint8_t*)aa)[i];
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