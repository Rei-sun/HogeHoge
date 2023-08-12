#pragma once

#include <stdint.h>

namespace HogeGen2 {

class IModuleDeserializer {
protected:
    virtual void Deserialize(uint8_t* data, int size);
};

}