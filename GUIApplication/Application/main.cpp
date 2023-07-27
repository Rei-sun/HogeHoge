#include <QApplication>
#include <QWidget>
#include <QTimer>
#include <QLabel>

#include <Client.h>
#include <Module.h>

EncoderModule encoderModule(1);
SensorModule sensorModule(1);
MotorControlModule motorControlModule(1);
SolenoidModule solenoidModule(1);

int count = 0;

void ReceiveCallBack(Client *cp, char *data, int size) {
    uint8_t module_id = data[0];
    uint8_t module_num = data[1];

    if (module_id  == (uint8_t)ModuleID::EncoderModule) {
        switch (module_num) {
            case 1: encoderModule.Deserialize((uint8_t*)data, size); break;
            default: break;
        }
    } else if (module_id  == (uint8_t)ModuleID::SensorModule) {
        switch (module_num) {
            case 1: sensorModule.Deserialize((uint8_t*)data, size); break;
            default: break;
        }
    } else if (module_id  == (uint8_t)ModuleID::MotorControlModule) {
        switch (module_num) {
            case 1: motorControlModule.Deserialize((uint8_t*)data, size); break;
            default: break;
        }
    } else if (module_id  == (uint8_t)ModuleID::SolenoidModule) {
        switch (module_num) {
            case 1: solenoidModule.Deserialize((uint8_t*)data, size); break;
            default: break;
        }
    }
    
    // int i;
    // for (i = 0; i < size - 1; i++) {
    //     printf("0x%02x, ", data[i]);
    // }
    // printf("0x%02x", data[i]);
}

int main(int argc, char *argv[]) {
    Client client;
    client.Init("127.0.0.1", 20000);
    client.Connect(ReceiveCallBack);

    QApplication app(argc, argv);
 
    QWidget *widget = new QWidget;
    QLabel *label = new QLabel(widget);
    label->setFixedSize(100,100);
    label->setText("0");

    QTimer aTimer;
    aTimer.setInterval(1000);
    //aTimer.setSingleShot(true);
    QObject::connect(&aTimer, &QTimer::timeout,
        [&]()
        {
            label->setText(QString::fromStdString(std::to_string(count++)));
            client.SendStr("Hoge EncoderModule 1");
            client.SendStr("Hoge SensorModule 1");
            client.SendStr("Hoge MotorControlModule 1");
            client.SendStr("Hoge SolenoidModuel 1");
            printf("timeout!!\n");
        });
    aTimer.start();
 

    widget->show();

    app.exec();

    client.Close();

    return 0;
}