#include "Application.h"
#include <HogeHoge.h>

using namespace HogeGen2;

void Application::Setup() {

}

void Application::Loop() {
    printf("p %d\n", ModuleManagerMain::sensorModules[0]->GetAnalog(1));
}