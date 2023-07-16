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
        hoge.serial.Send(1, 2, 3, 4, 1, data);
        printf("delta time = %f\n", timer.GetDeltaTime());
        timer.Sleep();
    }

    return 0;
}