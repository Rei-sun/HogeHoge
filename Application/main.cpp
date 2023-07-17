#include <HogeHoge.h>

#include <iostream>
#include <string>
#include <signal.h>

using namespace HogeHoge;

int main(){
    Timer timer;
    timer.SetHz(1);

    while (Hoge::Good()) {
        uint8_t test_data[] = { 3, 6, 9, 12 };
        Hoge::motor_control_module_1.Command(1, 1, 4, test_data);
        printf("delta time = %f\n", timer.GetDeltaTime());
        timer.Sleep();
    }

    return 0;
}