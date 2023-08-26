#include <QApplication>
#include <QTimer>

#include <MainWindow.h>

int count = 0;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    MainWindow main_window;

    // QTimer aTimer;
    // aTimer.setInterval(20);
    
    // QObject::connect(&aTimer, &QTimer::timeout,
    //     [&]()
    //     {
    //         ValueWithLabel<float>::UpdateAll();
    //     }
    // );
    // aTimer.start();

    // client.Close();

    return app.exec();
}