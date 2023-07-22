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
        
        Hoge::motor_control_module_1.SendBatch();
        Hoge::solenoid_module_1.SendBatch();

        end = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
        std::cout << "[" << std::chrono::duration_cast<std::chrono::milliseconds>(end.time_since_epoch()).count() << "]" << "processing time = " << elapsed << std::endl;

        timer.Sleep();
    }

    return 0;
}