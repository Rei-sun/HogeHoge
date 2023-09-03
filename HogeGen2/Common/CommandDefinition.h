#pragma once

namespace HogeGen2 {
    enum class CMD_MotorModule {
        Invalid,
        OK,
        SetAllDuty
    };

    enum class CMD_EncoderModule {
        Invalid,
        OK,
        GetLocalization,
        GetAllPulse
    };

    enum class CMD_SensorModule {
        Invalid,
        OK,
        GetSensorData
    };

    enum class CMD_SolenoidModule {
        Invalid,
        OK,
        SetAllState,
        SetState
    };

    enum class IPCMD {
        Undefined,
        ModuleCount = 0xff
    };
}

