#include <HogeHoge.h>

#include <iostream>
#include <string>
#include <signal.h>

using namespace HogeHoge;

int main(){
    Timer timer;
    timer.SetHz(1000);

    while (Hoge::Good()) {
        Hoge::encoder_module_1.SendGetLocalization();
        Hoge::encoder_module_1.SendGetAllPulse();
        Hoge::sensor_module_1.SendGetSensorData();

        printf("duty = %f\n", Hoge::motor_control_module_1.GetDuty(1));
        printf("delta time = %f\n", timer.GetDeltaTime());
        
        Hoge::motor_control_module_1.SendBatch();
        Hoge::solenoid_module_1.SendBatch();
        
        timer.Sleep();
    }

    return 0;
}