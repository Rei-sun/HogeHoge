#include <QApplication>
#include <QTimer>

#include <MainWindow.h>

int count = 0;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    MainWindow main_window;

    // client.Close();

    return app.exec();
}