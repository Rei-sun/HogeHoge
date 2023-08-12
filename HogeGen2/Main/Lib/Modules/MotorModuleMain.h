#pragma once

#include <Module.h>
//#include <IModuleReceiver.h>
#include <IModuleSender.h>
#include <IModuleSerializer.h>

namespace HogeGen2 {

class MotorModuleMain : public MotorModule, public IModuleSender, public IModuleSerializer {
    std::shared_ptr<uint8_t[]> serialized;
public:
    MotorModuleMain(uint8_t _module_num);
    void SendCommand(uint8_t cmd);
    std::pair<uint8_t, std::shared_ptr<uint8_t[]>> Serialized();
};

}