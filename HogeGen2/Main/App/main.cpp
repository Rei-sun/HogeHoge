#include <Application.h>
#include <HogeHoge.h>

#include <ConfigFileLoader.h>

using namespace HogeGen2;

int main(){
    Hoge::Init();
    ConfigFileLoader con;

    //-----------------------------
    // Setup Start
    //-----------------------------
    Application app;
    app.Setup();

    //-----------------------------
    // Setup End
    //-----------------------------

    Timer timer;
    timer.SetHz(100);

    while (Hoge::Good()) {

        // Request sensor value
        Hoge::GetSensorValue();

        //-----------------------------
        // Loop Start
        //-----------------------------
        app.Loop();

        //-----------------------------
        // Loop End
        //-----------------------------

        // Set actuator control
        Hoge::SetActuatorControl();

        //printf("%f\n", timer.GetProcessTime());

        // Sleep for loop
        timer.Sleep();

    }

    return 0;
}