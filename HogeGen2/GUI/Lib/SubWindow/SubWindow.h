#pragma once

#include <QMainWindow>
#include <QCoreApplication>
#include <QLabel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLineEdit>
#include <QPushButton>
#include <QCommandLineParser>
#include <QTextStream>

#include <iostream>

namespace HogeGen2 {

class SubWindow : public QWidget {
public:
    SubWindow():
        QWidget()
    {
        setWindowFlags(Qt::Window);
    }
};

}