#include <HogeHoge.h>

#include <iostream>
#include <string>
#include <signal.h>

using namespace HogeHoge;

int main(){
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

        // printf("duty = %f\n", Hoge::motor_control_module_1.GetDuty(1));
        // printf("delta time = %f\n", timer.GetDeltaTime());

        float potentio = Hoge::sensor_module_1.GetAnalog(1);
        float duty_potentio = potentio / 4095.f * 100.f;

        float duty_roll = Hoge::encoder_module_1.GetEulerRoll() / 90.0f * 100.f;

        bool button = Hoge::sensor_module_1.GetSwitch(1);

        Hoge::motor_control_module_1.SetDuty(1, abs(duty_roll));
        Hoge::solenoid_module_1.SetState(1, button);

        
        Hoge::motor_control_module_1.SendBatch();
        Hoge::solenoid_module_1.SendBatch();

        end = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
        std::cout << "[" << std::chrono::duration_cast<std::chrono::milliseconds>(end.time_since_epoch()).count() << "]" << "processing time = " << elapsed << std::endl;

        timer.Sleep();
    }

    return 0;
}