#pragma once

#include <Module.h>
#include <IModuleReceiver.h>
#include <IModuleSender.h>

namespace HogeGen2 {

class SensorModuleMain : public SensorModule, public IModuleReceiver, public IModuleSender {
public:
    SensorModuleMain(uint8_t _module_num);
    void ReceiveCommand(uint8_t cmd, uint8_t dev_id, uint8_t length, void *data);
    void SendCommand(uint8_t cmd);
};

}