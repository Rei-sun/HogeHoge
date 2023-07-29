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

EncoderModule encoderModule(1);
SensorModule sensorModule(1);
MotorControlModule motorControlModule(1);
SolenoidModule solenoidModule(1);

int count = 0;

void MakeEncoderModulePanel() {

}

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

    //printf("%f\n", encoderModule.GetPose(3));
    
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
    widget->setFixedSize(500, 500);
    widget->setStatusTip("Hello");

    // Pose
    QGroupBox *encoderGroup = new QGroupBox("Pose", widget);
    QVBoxLayout *vpose = new QVBoxLayout;
    ValueWithLabel<float> positionX(widget, "Position X", 0, 0, [&](){ return encoderModule.GetPose(1); });
    ValueWithLabel<float> positionY(widget, "Position Y", 0, 0, [&](){ return encoderModule.GetPose(2); });
    ValueWithLabel<float> roll(widget, "Roll", 0, 0, [&](){ return encoderModule.GetPose(3); });
    ValueWithLabel<float> pitch(widget, "Pitch", 0, 0, [&](){ return encoderModule.GetPose(4); });
    ValueWithLabel<float> yaw(widget, "Yaw", 0, 0, [&](){ return encoderModule.GetPose(5); });
    vpose->addLayout(positionX.GetLayout());
    vpose->addLayout(positionY.GetLayout());
    vpose->addLayout(roll.GetLayout());
    vpose->addLayout(pitch.GetLayout());
    vpose->addLayout(yaw.GetLayout());
    encoderGroup->setLayout(vpose);



    QGroupBox *sensorGroup = new QGroupBox("Sensor", widget);
    QVBoxLayout *vsensor = new QVBoxLayout;
    ValueWithLabel<short> analog1(widget, "Analog 1", 0, 0, [&](){ return sensorModule.GetAnalog(1); });
    ValueWithLabel<short> analog2(widget, "Analog 2", 0, 0, [&](){ return sensorModule.GetAnalog(2); });
    ValueWithLabel<short> analog3(widget, "Analog 3", 0, 0, [&](){ return sensorModule.GetAnalog(3); });
    ValueWithLabel<short> analog4(widget, "Analog 4", 0, 0, [&](){ return sensorModule.GetAnalog(4); });
    ValueWithLabel<short> analog5(widget, "Analog 5", 0, 0, [&](){ return sensorModule.GetAnalog(5); });
    ValueWithLabel<short> analog6(widget, "Analog 6", 0, 0, [&](){ return sensorModule.GetAnalog(6); });
    vsensor->addLayout(analog1.GetLayout());
    vsensor->addLayout(analog2.GetLayout());
    vsensor->addLayout(analog3.GetLayout());
    vsensor->addLayout(analog4.GetLayout());
    vsensor->addLayout(analog5.GetLayout());
    vsensor->addLayout(analog6.GetLayout());
    sensorGroup->setLayout(vsensor);



    QVBoxLayout *vBox = new QVBoxLayout;
    vBox->addWidget(encoderGroup);
    vBox->addWidget(sensorGroup);

    widget->setLayout(vBox);

    QLabel *label = new QLabel(widget);
    label->setFixedSize(100,20);
    label->setText("000.000");

    QTimer aTimer;
    aTimer.setInterval(20);
    
    QObject::connect(&aTimer, &QTimer::timeout,
        [&]()
        {
            label->setText(QString::fromStdString(std::to_string(count++)));
            client.SendStr(encoderModule.GetName().c_str());
            client.SendStr(sensorModule.GetName().c_str());
            client.SendStr(motorControlModule.GetName().c_str());
            client.SendStr(solenoidModule.GetName().c_str());
            
            ValueWithLabel<float>::UpdateAll();
        });
    aTimer.start();
 
    widget->show();

    //client.Close();

    return app.exec();
}