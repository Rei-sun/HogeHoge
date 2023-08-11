#pragma once

#include <Module.h>
//#include <IModuleReceiver.h>
#include <IModuleSender.h>

namespace HogeGen2 {

class SolenoidModuleMain : public SolenoidModule, public IModuleSender {
    bool wait_for_sub;
public:
    SolenoidModuleMain(uint8_t _module_num);
    void SendCommand(uint8_t cmd);
};

}
