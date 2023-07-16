#include <HogeHoge.h>

#include <iostream>
#include <string>
#include <signal.h>

using namespace HogeHoge;

int main(){
    Timer timer;
    timer.SetHz(1);

    while (Hoge::Good()) {
        uint8_t data[] = { 3 };
        hoge.motor_control_module_1.Command(0, 1, 1, data);
        printf("delta time = %f\n", timer.GetDeltaTime());
        timer.Sleep();
    }

    return 0;
}