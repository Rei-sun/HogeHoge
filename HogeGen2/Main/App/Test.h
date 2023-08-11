#pragma once

#include <HogeHoge.h>

class Test {
public:
    
    Test() {

    }

    void Setup() {

    }

    void Loop() {
        
        
        float potentio = HogeGen2::ModuleManager::GetSensorModules()[0]->GetAnalog(1);
        float duty_potentio = potentio / 4095.f * 100.f;

        float duty_roll = HogeGen2::ModuleManager::GetEncoderModules()[0]->GetPose(3) / 90.0f * 100.f;
        float duty_pitch = HogeGen2::ModuleManager::GetEncoderModules()[0]->GetPose(4) / 90.0f * 100.f;
        float duty_yaw = HogeGen2::ModuleManager::GetEncoderModules()[0]->GetPose(5) / 360.0f * 100.f;

        bool button1 = HogeGen2::ModuleManager::GetSensorModules()[0]->GetDigital(1);
        bool button2 = HogeGen2::ModuleManager::GetSensorModules()[0]->GetDigital(2);
        bool button3 = HogeGen2::ModuleManager::GetSensorModules()[0]->GetDigital(3);
        bool button4 = HogeGen2::ModuleManager::GetSensorModules()[0]->GetDigital(4);
        bool toggle_sw = HogeGen2::ModuleManager::GetSensorModules()[0]->GetDigital(5);
        
        HogeGen2::ModuleManager::GetMotorModules()[0]->SetDuty(1, abs(duty_roll));
        HogeGen2::ModuleManager::GetMotorModules()[0]->SetDuty(2, abs(duty_pitch));
        HogeGen2::ModuleManager::GetMotorModules()[0]->SetDuty(3, duty_potentio);

        HogeGen2::ModuleManager::GetSolenoidModules()[0]->SetState(1, button1);
        HogeGen2::ModuleManager::GetSolenoidModules()[0]->SetState(2, button2);
        HogeGen2::ModuleManager::GetSolenoidModules()[0]->SetState(3, button3);

        
    }
};