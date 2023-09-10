#include <Application.h>
#include <HogeHoge.h>

#include <ConfigFileLoader.h>

#include <MessageOutputter.h>

using namespace HogeGen2;

int main(){
    Hoge::Init();
    ConfigFileLoader con;

    //MessageOutputter output(&std::cerr);
    //MessageOutputter output("test.txt");

    //output.Message("Test");

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

        // 処理にかかった時間を表示する。
        //log_output.DebugMessage("%f", timer.GetProcessTime());

        // Sleep for loop
        timer.Sleep();

    }

    return 0;
}