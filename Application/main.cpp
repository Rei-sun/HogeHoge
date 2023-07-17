#include <HogeHoge.h>

#include <iostream>
#include <string>
#include <signal.h>

using namespace HogeHoge;

int main(){
    Timer timer;
    timer.SetHz(1);

    while (Hoge::Good()) {
        Hoge::motor_control_module_1.SetDuty(1, 50.0f);
        Hoge::motor_control_module_1.SendBatch();
        printf("duty = %f\n", Hoge::motor_control_module_1.GetDuty(1));
        printf("delta time = %f\n", timer.GetDeltaTime());
        timer.Sleep();
    }

    return 0;
}