#pragma once

#include <QLabel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <Module.h>
#include <ModuleManager.h>

#include <DeviceValue.h>
#include <ModuleManager.h>

class MainWindow : public QWidget {
public:
    std::string pose_str[5];
    std::string pulse_str[4];
    std::string analog_str[6];
    std::string digital_str[6];
    std::string duty_str[6];
    std::string solenoid_str[8];

    QGroupBox *encoderGroup;
    QVBoxLayout *vpose;
    ValueWithLabel<float>* pose_values[5];
    QVBoxLayout *vpulse;
    ValueWithLabel<float>* pulse_values[4];

    QGroupBox *sensorGroup;
    QHBoxLayout *hsensor;
    QVBoxLayout *vanalog;
    ValueWithLabel<short>* analog_values[6];
    QVBoxLayout *vdigital;
    ValueWithLabel<short>* digital_values[6];

    QGroupBox *motorGroup;
    QVBoxLayout *vduty;
    ValueWithLabel<float>* duty_values[6];

    QGroupBox *solenoidGroup;
    QVBoxLayout *vsolenoid;
    ValueWithLabel<short>* solenoid_values[8];

    QVBoxLayout *vBox;
    MainWindow() : 
        pose_str {
            "Position X",
            "Position Y",
            "Roll",
            "Pitch",
            "Yaw" 
        },
        pulse_str {
            "Pulse 1",
            "Pulse 2",
            "Pulse 3",
            "Pulse 4"
        },
        analog_str {
            "Analog 1",
            "Analog 2",
            "Analog 3",
            "Analog 4",
            "Analog 5",
            "Analog 6",
        },
        digital_str {
            "Digital 1",
            "Digital 2",
            "Digital 3",
            "Digital 4",
            "Digital 5",
            "Digital 6"
        },
        duty_str {
            "Duty 1",
            "Duty 2",
            "Duty 3",
            "Duty 4",
            "Duty 5",
            "Duty 6",
        },
        solenoid_str {
            "Solenoid 1",
            "Solenoid 2",
            "Solenoid 3",
            "Solenoid 4",
            "Solenoid 5",
            "Solenoid 6",
            "Solenoid 7",
            "Solenoid 8"
        }
    {
        encoderGroup = new QGroupBox("Pose", this);
        vpose = new QVBoxLayout;
        for (int i = 0; i < 5; i++) {
            pose_values[i] = new ValueWithLabel<float>(dynamic_cast<QWidget*>(this), pose_str[i].c_str(), 0, 0, [=](){
                return ModuleManager::GetEncoderModules()[0]->GetPose(i + 1);
            });
            vpose->addLayout(pose_values[i]->GetLayout());
        }
        encoderGroup->setLayout(vpose);

        sensorGroup = new QGroupBox("Sensor", this);
        hsensor = new QHBoxLayout;
        vanalog = new QVBoxLayout;
        for (int i = 0; i < 6; i++) {
            analog_values[i] = new ValueWithLabel<short>(dynamic_cast<QWidget*>(this), analog_str[i].c_str(), 0, 0, [=](){
                return ModuleManager::GetSensorModules()[0]->GetAnalog(i + 1);
            });
            vanalog->addLayout(analog_values[i]->GetLayout());
        }
        hsensor->addLayout(vanalog);

        vdigital = new QVBoxLayout;
        for (int i = 0; i < 6; i++) {
            digital_values[i] = new ValueWithLabel<short>(dynamic_cast<QWidget*>(this), digital_str[i].c_str(), 0, 0, [=](){
                return ModuleManager::GetSensorModules()[0]->GetDigital(i + 1);
            });
            vdigital->addLayout(digital_values[i]->GetLayout());
        }
        hsensor->addLayout(vdigital);

        sensorGroup->setLayout(hsensor);

        motorGroup = new QGroupBox("Motor", this);
        vduty = new QVBoxLayout;
        for (int i = 0; i < 6; i++) {
            duty_values[i] = new ValueWithLabel<float>(dynamic_cast<QWidget*>(this), duty_str[i].c_str(), 0, 0, [=](){
                return ModuleManager::GetMotorControlModules()[0]->GetDuty(i + 1);
            });
            vduty->addLayout(duty_values[i]->GetLayout());
        }
        motorGroup->setLayout(vduty);

        solenoidGroup = new QGroupBox("Solenoid", this);
        vsolenoid = new QVBoxLayout;
        for (int i = 0; i < 8; i++) {
            solenoid_values[i] = new ValueWithLabel<short>(dynamic_cast<QWidget*>(this), solenoid_str[i].c_str(), 0, 0, [=](){
                return ModuleManager::GetSolenoidModules()[0]->GetState(i + 1);
            });
            vsolenoid->addLayout(solenoid_values[i]->GetLayout());
        }
        solenoidGroup->setLayout(vsolenoid);

        vBox = new QVBoxLayout;
        vBox->addWidget(encoderGroup);
        vBox->addWidget(sensorGroup);
        vBox->addWidget(motorGroup);
        vBox->addWidget(solenoidGroup);
        this->setLayout(vBox);
    }
};