#pragma once

#include <stdint.h>

namespace HogeGen2 {

class IModuleSender {
public:
    virtual void SendCommand(uint8_t cmd) = 0;
};

}