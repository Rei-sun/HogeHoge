#include <HogeHoge.h>

#include <iostream>
#include <string>
#include <signal.h>

#include <GUICommunication/GUICommunication.h>

using namespace HogeHoge;

int main(){
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
        Hoge::encoder_module_1.SendGetLocalization();
        Hoge::encoder_module_1.SendGetAllPulse();
        Hoge::sensor_module_1.SendGetSensorData();

        // printf("delta time = %f\n", timer.GetDeltaTime());

        float potentio = Hoge::sensor_module_1.GetAnalog(1);
        float duty_potentio = potentio / 4095.f * 100.f;

        float duty_roll = Hoge::encoder_module_1.GetEulerRoll() / 90.0f * 100.f;
        float duty_pitch = Hoge::encoder_module_1.GetEulerPitch() / 90.0f * 100.f;
        float duty_yaw = Hoge::encoder_module_1.GetEulerYaw() / 360.0f * 100.f;

        bool button1 = Hoge::sensor_module_1.GetSwitch(1);
        bool toggle_sw = Hoge::sensor_module_1.GetSwitch(5);

        Hoge::motor_control_module_1.SetDuty(1, abs(duty_roll));
        Hoge::motor_control_module_1.SetDuty(2, abs(duty_pitch));
        Hoge::motor_control_module_1.SetDuty(3, abs(duty_yaw));

        Hoge::solenoid_module_1.SetState(1, button1);
        Hoge::solenoid_module_1.SetState(3, toggle_sw);
        
        Hoge::motor_control_module_1.SendBatch();
        Hoge::solenoid_module_1.SendBatch();

        end = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
        // std::cout << "[" << std::chrono::duration_cast<std::chrono::milliseconds>(end.time_since_epoch()).count() << "]" << "processing time = " << elapsed << std::endl;

        timer.Sleep();
    }

    return 0;
}