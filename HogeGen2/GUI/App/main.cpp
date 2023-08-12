#include <QApplication>
#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <Client.h>
#include <Module.h>
#include <ModuleManager.h>

#include <DeviceValue.h>

#include <MainWindow.h>

int count = 0;

void ReceiveCallBack(Client *cp, char *data, int size) {
    uint8_t module_id = data[0];
    uint8_t module_num = data[1];

    ModuleManager::ReceiveData(module_id, module_num, (uint8_t*)data, size);
    
    // int i;
    // for (i = 0; i < size - 1; i++) {
    //     printf("0x%02x, ", data[i]);
    // }
    // printf("0x%02x", data[i]);
}

int main(int argc, char *argv[]) {
    ModuleManager::MakeMotorControlModule(1);
    ModuleManager::MakeEncoderModule(1);
    ModuleManager::MakeSensorModule(1);
    ModuleManager::MakeSolenoidModule(1);

    Client client;
    client.Init("127.0.0.1", 20000);
    client.Connect(ReceiveCallBack);

    QApplication app(argc, argv);

    MainWindow *widget = new MainWindow;

    widget->setFixedSize(800, 800);

    QTimer aTimer;
    aTimer.setInterval(20);
    
    QObject::connect(&aTimer, &QTimer::timeout,
        [&]()
        {
            for (auto encoderModule : ModuleManager::GetEncoderModules()) {
                client.SendStr(encoderModule->GetName().c_str());
            }
            for (auto sensorModule : ModuleManager::GetSensorModules()) {
                client.SendStr(sensorModule->GetName().c_str());
            }
            for (auto motorControlModule : ModuleManager::GetMotorControlModules()) {
                client.SendStr(motorControlModule->GetName().c_str());
            }
            for (auto solenoidModule : ModuleManager::GetSolenoidModules()) {
                client.SendStr(solenoidModule->GetName().c_str());
            }
            
            ValueWithLabel<float>::UpdateAll();
        });
    aTimer.start();
 
    widget->show();

    // client.Close();

    return app.exec();
}