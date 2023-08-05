#pragma once

#include <HogeHogeSerial.h>
#include <string.h>

namespace HogeHoge {
    class BaseModule {
    protected:
        ModuleID module_id;
        uint8_t module_num;
        HogeHogeSerial &serial;
        bool wait_for_response;
        std::shared_ptr<uint8_t[]> serialized;

        BaseModule(HogeHogeSerial &_serial, ModuleID _module_id, uint8_t _module_num):
            serial(_serial), module_id(_module_id), module_num(_module_num), wait_for_response(false), serialized(nullptr)
        {

        }
        BaseModule() = delete;

        virtual bool Command(uint8_t cmd, uint8_t device_id, uint8_t length, void* data) = 0;
        virtual void Receive(uint8_t cmd, uint8_t device_id, uint8_t length, void* data) {}
        virtual void SendBatch(){};
        virtual std::pair<uint8_t, std::shared_ptr<uint8_t[]>> Serialized() = 0;
    };
}