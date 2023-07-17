#pragma once
#include <HogeHogeSerial.h>

namespace HogeHoge {
    class BaseModule {
    protected:
        ModuleID module_id;
        uint8_t module_num;
        HogeHogeSerial &serial;

        BaseModule(HogeHogeSerial &_serial, ModuleID _module_id, uint8_t _module_num):
            serial(_serial), module_id(_module_id), module_num(_module_num)
        {

        }
        BaseModule() = delete;

        virtual bool Command(uint8_t cmd, uint8_t device_id, uint8_t length, void* data) = 0;
        virtual void Receive(uint8_t cmd, uint8_t device_id, uint8_t length, void* data) {}
    };
}