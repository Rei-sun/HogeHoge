#pragma once
#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <string>
#include <sstream>
#include <iomanip>
#include <functional>

class BaseDeviceValue {
public:
    static std::vector<BaseDeviceValue*> values;
    virtual void Update() = 0;
};

template<typename T> class DeviceValue : public BaseDeviceValue {
public:
    std::function<T()> func;
};

template<typename T> class DeviceValueLabel : public QLabel, public DeviceValue<T> {
    DeviceValueLabel(QWidget* widget, int ax, int ay, std::function<T()> f) : QLabel(widget), DeviceValue<T>() {
        DeviceValue<T>::func = f;
        setFixedSize(50,20);
        setText("000.000");
    }

    void Update() {
    }
};

template<> class DeviceValueLabel<float> : public QLabel, public DeviceValue<float> {
public:
    DeviceValueLabel(QWidget* widget, int ax, int ay, std::function<float()> f) : QLabel(widget), DeviceValue<float>() {
        DeviceValue<float>::func = f;
        setFixedSize(50,20);
        setText("000.000");
    }

    void Update() {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(3) << DeviceValue<float>::func();
        setText(QString::fromStdString(ss.str()));
    }
};

template<> class DeviceValueLabel<short> : public QLabel, public DeviceValue<short> {
public:
    DeviceValueLabel(QWidget* widget, int ax, int ay, std::function<short()> f) : QLabel(widget), DeviceValue<short>() {
        DeviceValue<short>::func = f;
        setFixedSize(50,20);
        setText("00000");
    }

    void Update() {
        std::stringstream ss;
        ss << std::setw(5) << DeviceValue<short>::func();
        setText(QString::fromStdString(ss.str()));
    }
};

template<typename T> class ValueWithLabel : public DeviceValueLabel<T> {
    QHBoxLayout *hbox;
    QLabel *label;
public:
    ValueWithLabel(QWidget* widget, const char * text, int ax, int ay, std::function<T()> f) :
        DeviceValueLabel<T>(widget, ax, ay, f)
    {
        hbox = new QHBoxLayout;
        label = new QLabel(text, widget);
        hbox->addWidget(label);
        hbox->addWidget(this);
        BaseDeviceValue::values.push_back(this);
    }
    QHBoxLayout *GetLayout() {
        return hbox;
    }
    static void UpdateAll() {
        for (auto i : BaseDeviceValue::values) {
            i->Update();
        }
    }
};