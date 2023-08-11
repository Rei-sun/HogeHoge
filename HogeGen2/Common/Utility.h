#pragma once

#include <algorithm>
#include <stdint.h>
#include <string.h>

namespace HogeGen2 {

// template は暗黙的に inline になるぞい
template<typename... Args> void SetArrays(Args... args) {
    for(auto t : std::initializer_list<std::tuple<void*, void*, int>>{args...}) {
        memcpy(std::get<0>(t), std::get<1>(t), std::get<2>(t));
    }
}

template<typename T> T Clamp(T v, T low, T high) {
    return std::min(std::max(v, low), high);
}

inline uint8_t CheckSum(uint8_t *data, size_t size) {
    uint8_t checksum = 0;
    for (int i = 0; i < size - 1; i++) {
      checksum += data[i];
    }
    return checksum;
}

}