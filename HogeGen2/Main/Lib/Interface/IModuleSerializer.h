#pragma once

#include <stdint.h>
#include <utility>
#include <memory>

namespace HogeGen2 {

class IModuleSerializer {
public:
    virtual std::pair<uint8_t, std::shared_ptr<uint8_t[]>> Serialized() = 0;
};

}