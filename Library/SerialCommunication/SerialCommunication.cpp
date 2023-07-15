#include "SerialCommunication.h"

void SerialCommunication::StartCommunicationThread() {
    if (thread_continue) {
        printf("already thread running.\n");
    } else {
        thread_continue = true;
        thread = std::shared_ptr<std::thread>(new std::thread(&SerialCommunication::CommunicationProcess, this));
    }
}

void SerialCommunication::StopCommunicationThread() {
    if (thread_continue) {
        thread_continue = false;
        if (thread != nullptr) {
            thread.get()->join();
        }
    } else {
        printf("already thread stopped.\n");
    }
}

void SerialCommunication::CommunicationProcess() {
    // 読み込みバッファサイズ
    static const size_t read_buffer_size = 1024;
    // バッファ 
    uint8_t *read_buffer = new uint8_t[read_buffer_size];
    
    fd_set rfds;
    struct timeval tv;

    while(thread_continue){
        if (is_connected) {
            // 通信処理
            FD_ZERO(&rfds);
            FD_SET(fd, &rfds);
            tv.tv_sec = timeout_sec;
            tv.tv_usec = timeout_usec;
            int ret = select(fd + 1, &rfds, NULL, NULL, &tv);
            if (ret > 0) {
                // OK
                if (FD_ISSET(fd, &rfds)){
                    // read
                    auto readed = read(fd, read_buffer, read_buffer_size);
                    if (readed > 0) {
                        // recieve
                        printf("readed %ld, %s\n", readed, read_buffer);
                    } else if (readed == 0) {
                        // disconnect
                        CloseSerialPort(true);
                    } else {
                        printf("read() error\n");
                        std::quick_exit(EXIT_FAILURE);
                    }
                }
            } else if (ret < 0) {
                // Error
                printf("error\n");
                if (errno == EINTR) continue;
                // その他のエラーの場合、終了する。
                std::quick_exit(EXIT_FAILURE);
            } else {
                // Timeout
                //printf("timeout\n");
            }
        } else {
            // 再接続処理
            printf("reconnect %s...\n", device_name.c_str());
            sleep(reconnect_interval);
            OpenSerialPort(device_name.c_str(), true);
        }
    }

    delete[] read_buffer;
}

bool SerialCommunication::OpenSerialPort(const char *device_name, bool reconnect = false) {
    std::lock_guard<std::mutex> lock(io_mutex);

    // open
    fd = open(device_name, O_RDWR | O_NOCTTY | O_NONBLOCK);
    
    // check
    if(fd < 0) {
        return false;
    }

    fcntl(fd, F_SETFL,0);
    
    //load configuration
    struct termios config_tio;
    tcgetattr(fd, &config_tio);
    
    //set baudrate
    speed_t BAUDRATE = B1000000;
    cfsetispeed(&config_tio, BAUDRATE);
    cfsetospeed(&config_tio, BAUDRATE);

    //non canonical, non echo back
    config_tio.c_lflag &= ~(ECHO | ICANON);
    
    //non blocking
    config_tio.c_cc[VMIN] = 0;
    config_tio.c_cc[VTIME] = 0;
    
    //store configuration
    tcsetattr(fd, TCSANOW, &config_tio);

    transmit_failure_count = 0;
    is_connected = true;
    this->device_name = device_name;

    printf("serial open.\n");

    if (reconnect) {
        if (on_reconnected != nullptr) on_reconnected();
    } else {
        if (on_connect != nullptr) on_connect();
    }

    return true;
}

void SerialCommunication::CloseSerialPort(bool error = false) {
    std::lock_guard<std::mutex> lock(io_mutex);
    
    if (fd < 0 || !is_connected) {
        return;
    }

    close(fd);
    fd = -1;
    is_connected = false;

    printf("serial close.\n");

    if (error) {
        if (on_disconnected != nullptr) on_disconnected();
    } else {
        if (on_disconnect != nullptr) on_disconnect();
    }

}   

SerialCommunication::SerialCommunication():
    device_name(""),
    fd(-1),
    on_connect([](){}),
    on_disconnect([](){}),
    on_disconnected([](){}),
    thread(nullptr),
    thread_continue(false),
    is_connected(false),
    transmit_failure_count(0),
    timeout(1000),
    timeout_sec(timeout / 1000),
    timeout_usec((timeout % 1000) * 1000),
    reconnect_interval(1),
    io_mutex()
{

}

SerialCommunication::~SerialCommunication(){
    if (is_connected) {
        Close();
    }
}

bool SerialCommunication::Open(const char *device_name) {
    if (is_connected) return true;
    if (OpenSerialPort(device_name)) {
        StartCommunicationThread();
        return true;
    }
    return false;
}

bool SerialCommunication::Open(std::string device_name) {
    return Open(device_name.c_str());
}

void SerialCommunication::Close() {
    StopCommunicationThread();
    CloseSerialPort();
}

void SerialCommunication::Transmit(uint8_t *data, size_t size) {
    if (!is_connected) return;
    int32_t written = write(fd, data, size);
    if (written < 0) {
        // Failed
        transmit_failure_count++;
        printf("transmit failure\n");
    } else {
        if (transmit_failure_count > 0) {
            transmit_failure_count--;
        }
        transmit_failure_count = false;
        printf("transmit %d byte\n", written);
    }
}

void SerialCommunication::Transmit(const char* msg, size_t size) {
    if (!is_connected) return;
    Transmit((uint8_t*)msg, size);
}

void SerialCommunication::Transmit(std::string msg) {
    if (!is_connected) return;
    // msg.size() + 1 は 文字数 + ヌル文字
    Transmit(msg.c_str(), msg.size() + 1);
}

void SerialCommunication::RegisterCallbackOnConnect(std::function<void(void)> callback) {
    on_connect = callback;
}

void SerialCommunication::RegisterCallbackOnDisconnect(std::function<void(void)> callback) {
    on_disconnect = callback;
}

void SerialCommunication::RegisterCallbackOnDisconnected(std::function<void(void)> callback) {
    on_disconnected = callback;
}

void SerialCommunication::RegisterCallbackOnReconnected(std::function<void(void)> callback) {
    on_reconnected = callback;
}