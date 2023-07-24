#include <QApplication>
#include <QWidget>
#include <QTimer>
#include <QLabel>

int count = 0;

int main(int argc, char *argv[]) {
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
            printf("timeout!!\n");
        });
    aTimer.start();
 

    widget->show();

    return app.exec();
}