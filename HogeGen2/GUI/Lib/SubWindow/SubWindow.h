#pragma once

#include <ModuleManagerGUI.h>

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

using namespace HogeGen2;

class SubWindow : public QWidget {
public:
    SubWindow(QWidget* parent):
        QWidget(parent)
    {
        setWindowFlags(Qt::Window);
        setLayout(new QVBoxLayout());
        layout()->addWidget(new QLabel("AAA"));
        resize(100,100);
    }
};