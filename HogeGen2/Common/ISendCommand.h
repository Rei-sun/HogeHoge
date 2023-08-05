#pragma once

#include <stdint.h>

class ISendCommand {
    virtual bool Command(uint8_t cmd, uint8_t device_id, uint8_t length, void* data) = 0;
};