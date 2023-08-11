#pragma once

#include <Module.h>
//#include <IModuleReceiver.h>
#include <IModuleSender.h>

namespace HogeGen2 {

class MotorModuleMain : public MotorModule, public IModuleSender {
public:
    MotorModuleMain(uint8_t _module_num);
    void SendCommand(uint8_t cmd);
};

}