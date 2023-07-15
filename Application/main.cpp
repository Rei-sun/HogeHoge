#include <iostream>
#include <SerialCommunication.h>
#include <string>
#include <signal.h>

void abort_handler(int sig) {
    exit(0);
}

void exit_handler() {
    printf("Hello world\n");
}

void register_abort() {
    if ( signal(SIGINT, abort_handler) == SIG_ERR ) {
        exit(1);
    }
}

void register_exit() {
    atexit(exit_handler);
}

int main(){
    register_abort();
    register_exit();

    std::string target = "/dev/ESP32-WROOM-32E";
    SerialCommunication serial;
    serial.RegisterCallbackOnConnect([](){printf("いぇーい、接続できたよぉぉぉ！\n");});
    serial.RegisterCallbackOnDisconnect([](){printf("いぇーい、切断したよぉぉぉ！\n");});
    serial.RegisterCallbackOnDisconnected([](){printf("接続が切れましたが？\n");});
    serial.RegisterCallbackOnReconnected([](){printf("いぇーい、再接続されましたぁぁぁ!\n");});

    if (!serial.Open(target)) {
        std::cout << "Cannot connect " << target << std::endl;
        std::quick_exit(0);
    }

    for(;;) {
        serial.Transmit("test");
        sleep(1);
    }

    return 0;
}