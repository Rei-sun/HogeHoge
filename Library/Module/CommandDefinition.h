#pragma once

namespace HogeHoge {
    enum class CMD_MotorControlModule {
        SetAllDuty
    };

    enum class CMD_EncoderModule {
        GetLocalization,
        GetAllPulse
    };

    enum class CMD_SensorModule {
        GetSensorData
    };

    enum class CMD_SolenoidModule {
        SetAllState,
        SetState
    };
}

