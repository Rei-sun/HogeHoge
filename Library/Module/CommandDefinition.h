#pragma once

namespace HogeHoge {
    enum class CMD_MotorControlModule {
        SetAllDuty,
        SetDuty
    };

    enum class CMD_EncoderModule {
        GetLocalization,
        GetAllPulse
    };

    enum class CMD_SensorModule {
        GetDigital,
        GetAnalog
    };

    enum class CMD_SolenoidModule {
        SetAllState,
        SetState
    };
}

