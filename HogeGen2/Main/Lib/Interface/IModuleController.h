#pragma once

#include <stdint.h>
#include <utility>
#include <memory>

namespace HogeGen2 {

class IModuleController {
public:
    virtual void Control(uint8_t id, float value) = 0;
};

}