#include <HogeHoge.h>

#include <iostream>
#include <string>
#include <signal.h>

#include <GUICommunication/GUICommunication.h>

using namespace HogeGen2;

int main(){
    Hoge::Init();
    GUICommunication gcommu(20000);
    
    printf("pid = %d\n", getpid());

    Timer timer;
    timer.SetHz(100);

    // start measurement time point
    std::chrono::system_clock::time_point start;
    
    // end measurement time point
    std::chrono::system_clock::time_point end;

    while (Hoge::Good()) {
        start = std::chrono::system_clock::now();

        Hoge::GetSensorValue();
        
        float potentio = ModuleManager::GetSensorModules()[0]->GetAnalog(1);
        float duty_potentio = potentio / 4095.f * 100.f;

        float duty_roll = ModuleManager::GetEncoderModules()[0]->GetPose(3) / 90.0f * 100.f;
        float duty_pitch = ModuleManager::GetEncoderModules()[0]->GetPose(4) / 90.0f * 100.f;
        float duty_yaw = ModuleManager::GetEncoderModules()[0]->GetPose(5) / 360.0f * 100.f;

        bool button1 = ModuleManager::GetSensorModules()[0]->GetDigital(1);
        bool button2 = ModuleManager::GetSensorModules()[0]->GetDigital(2);
        bool button3 = ModuleManager::GetSensorModules()[0]->GetDigital(3);
        bool button4 = ModuleManager::GetSensorModules()[0]->GetDigital(4);
        bool toggle_sw = ModuleManager::GetSensorModules()[0]->GetDigital(5);
        
        ModuleManager::GetMotorModules()[0]->SetDuty(1, abs(duty_roll));
        ModuleManager::GetMotorModules()[0]->SetDuty(2, abs(duty_pitch));
        ModuleManager::GetMotorModules()[0]->SetDuty(3, duty_potentio);

        ModuleManager::GetSolenoidModules()[0]->SetState(1, button1);
        ModuleManager::GetSolenoidModules()[0]->SetState(2, button2);
        ModuleManager::GetSolenoidModules()[0]->SetState(3, button3);

        Hoge::SetActuatorControl();

        end = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
        std::cout << "[" << std::chrono::duration_cast<std::chrono::milliseconds>(end.time_since_epoch()).count() << "]" << "processing time = " << elapsed << std::endl;

        timer.Sleep();
    }

    return 0;
}