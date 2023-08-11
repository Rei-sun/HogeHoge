#pragma once

#include <stdint.h>

namespace HogeGen2 {

class IModuleReceiver {
protected:
    virtual void ReceiveCommand(uint8_t cmd, uint8_t dev_id, uint8_t length, void *data) = 0;
};

}