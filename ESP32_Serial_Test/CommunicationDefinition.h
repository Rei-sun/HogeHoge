#pragma once

namespace HogeHoge {
    enum class CMD_MotorControlModule {
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
}

