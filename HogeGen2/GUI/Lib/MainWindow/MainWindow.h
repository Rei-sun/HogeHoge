#pragma once

#include <IPCommunicationSub.h>

#include <QMainWindow>
// #include <QCoreApplication>
// #include <QLabel>
#include <QGroupBox>
// #include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
// #include <QLineEdit>
// #include <QPushButton>
#include <QCommandLineParser>
#include <QTextStream>
#include <QTimer>
#include <EncoderModuleGUI.h>
#include <MotorModuleGUI.h>
#include <SensorModuleGUI.h>
#include <SolenoidModuleGUI.h>
#include <ModuleManagerGUI.h>
#include <iostream>
#include <future>
#include <IGroupBox.h>
#include <ModuleManagerGUI.h>

namespace HogeGen2 {

    class MainWindow : public QMainWindow {
        static inline int port = 20000;
        static inline int mode = 0;   // 0: OFFLINE, 1:ONLINE
        QTimer Timer;
        QLabel *connect_label;
        QPushButton *connect_button;

        QScrollArea *CreateBaseScrollArea();
        QGroupBox *CreateGroupBox(QString str, std::vector<IGroupBox*> modules);
        template<typename T> QGroupBox *CreateModuleGroupBox(QString str, std::vector<T*> modules);
        void SetTimer();
        void TimerStart();
        void TimerStop();
    public:
        static inline IPCommunicationSub ip_communication = IPCommunicationSub();
        MainWindow();
        void Setup();
        bool CommandLineAnalyze(int argc, QStringList argv);
        static void TimerUpdate();
        void ConnectButton_OnClicked();
    };

}
