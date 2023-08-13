#pragma once

#include <Module.h>
//#include <IModuleReceiver.h>
#include <IModuleSender.h>
#include <IModuleSerializer.h>
#include <IModuleController.h>

namespace HogeGen2 {

class MotorModuleMain : public MotorModule, public IModuleSender, public IModuleSerializer, public IModuleController {
    std::shared_ptr<uint8_t[]> serialized;
public:
    MotorModuleMain(uint8_t _module_num);
    void SendCommand(uint8_t cmd);
    std::pair<uint8_t, std::shared_ptr<uint8_t[]>> Serialized();
    void Control(uint8_t id, float value);
};

}