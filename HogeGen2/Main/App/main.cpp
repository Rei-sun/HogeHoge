#include <HogeHoge.h>

#include <iostream>
#include <string>
#include <signal.h>

#include <GUICommunication/GUICommunication.h>

using namespace HogeGen2;

int main(){
    Hoge::Init();
    //GUICommunication gcommu(20000);
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
        
        printf("p %d\n", ModuleManagerMain::sensorModules[0]->GetAnalog(1));

        Hoge::SetActuatorControl();

        end = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
        std::cout << "[" << std::chrono::duration_cast<std::chrono::milliseconds>(end.time_since_epoch()).count() << "]" << "processing time = " << elapsed << std::endl;

        timer.Sleep();
    }

    return 0;
}