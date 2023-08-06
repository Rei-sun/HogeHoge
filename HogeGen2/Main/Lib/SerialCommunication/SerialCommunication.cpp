#include "SerialCommunication.h"

using namespace HogeGen2;

void SerialCommunication::StartCommunicationThread() {
    if (thread_continue) {
        printf("already thread running.\n");
    } else {
        thread_continue = true;
        thread = std::shared_ptr<std::thread>(new std::thread(&SerialCommunication::CommunicationProcess, this));
        // 最優先
        struct sched_param param;
        param.sched_priority = 99;
        if (pthread_setschedparam(thread.get()->native_handle(), SCHED_RR, &param) != 0)
            printf("Error Priority set\n");
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
    // Read buffer size
    static const size_t read_buffer_size = 1024;
    // Read buffer
    uint8_t *read_buffer = new uint8_t[read_buffer_size];

    auto thread_id = std::this_thread::get_id();
    
    fd_set rfds;
    struct timeval tv;

    // そういう問題ではなかった
    struct serial_struct serial_setting;
    ioctl(fd, TIOCGSERIAL, &serial_setting);
    serial_setting.flags |= ASYNC_LOW_LATENCY;
    ioctl(fd, TIOCSSERIAL, &serial_setting);
    int nread = 0;
    ioctl(fd, FIONREAD, &nread);

    while(thread_continue){
        if (is_connected) {
            FD_ZERO(&rfds);
            FD_SET(fd, &rfds);
            tv.tv_sec = timeout_sec;
            tv.tv_usec = timeout_usec;
            int ret = select(fd + 1, &rfds, NULL, NULL, &tv);
            if (ret > 0) {
                // OK
                if (FD_ISSET(fd, &rfds)){
                    // Read
                    auto readed = read(fd, read_buffer, read_buffer_size);
                    if (readed > 0) {
                        // Recieve
                        if (on_receive != nullptr) {
                            on_receive(read_buffer, readed);
                        }
                    } else if (readed == 0) {
                        // Disconnect
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
                // Other error
                std::quick_exit(EXIT_FAILURE);
            } else {
                // Timeout
                if (on_timeout != nullptr) {
                    on_timeout();
                }
            }
        } else {
            // Reconnect process
            printf("reconnect %s...\n", device_name.c_str());
            sleep(reconnect_interval);
            OpenSerialPort(device_name.c_str(), true);
        }
    }

    delete[] read_buffer;
}

bool SerialCommunication::OpenSerialPort(const char *device_name, bool reconnect = false) {
    std::lock_guard<std::mutex> lock(io_mutex);

    this->device_name = device_name;

    // open
    //fd = open(device_name, O_RDWR | O_NOCTTY | O_NONBLOCK);
    fd = open(device_name, O_RDWR | O_NOCTTY);
    
    // check
    if(fd < 0) {
        return false;
    }

    fcntl(fd, F_SETFL,0);
    
    struct termios config_tio;
    
    // Get
    tcgetattr(fd, &config_tio);
    
    // Configure
    cfmakeraw(&config_tio);
    
    speed_t BAUDRATE = B2000000;
    cfsetispeed(&config_tio, BAUDRATE);
    cfsetospeed(&config_tio, BAUDRATE);

    // config_tio.c_iflag = 0;
    // config_tio.c_oflag = 0;
    // config_tio.c_lflag = 0;
    
    // config_tio.c_cc[VMIN] = 0;
    // config_tio.c_cc[VTIME] = 0;
    
    // Flush
    tcflush(fd, TCIOFLUSH);

    // Set
    tcsetattr(fd, TCSANOW, &config_tio);

    transmit_failure_count = 0;
    is_connected = true;

    printf("serial open.\n");

    if (reconnect) {
        if (on_reconnect != nullptr) on_reconnect();
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
        if (on_disconnect != nullptr) on_disconnect();
    } else {
        if (on_close != nullptr) on_close();
    }

}   

SerialCommunication::SerialCommunication():
    device_name(""),
    fd(-1),
    on_connect(nullptr),
    on_close(nullptr),
    on_receive(nullptr),
    on_disconnect(nullptr),
    on_reconnect(nullptr),
    on_timeout(nullptr),
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
    } else {
        StopCommunicationThread();
    }
}

bool SerialCommunication::Open(const char *device_name, bool thread_start = false) {
    if (is_connected) return true;

    bool ret = OpenSerialPort(device_name); 
    if (ret || thread_start) {
        StartCommunicationThread();
        return ret;
    }
    return false;
}

bool SerialCommunication::Open(std::string device_name, bool thread_start) {
    return Open(device_name.c_str(), thread_start);
}

void SerialCommunication::Close() {
    StopCommunicationThread();
    CloseSerialPort();
}

bool SerialCommunication::IsConnect() {
    return is_connected;
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
        // printf("transmit %d byte\n", written);
    }
}

void SerialCommunication::Transmit(const char* msg, size_t size) {
    if (!is_connected) return;
    Transmit((uint8_t*)msg, size);
}

void SerialCommunication::Transmit(std::string msg) {
    if (!is_connected) return;
    // msg.size() + 1 is number of char + null
    Transmit(msg.c_str(), msg.size() + 1);
}

void SerialCommunication::RegisterCallbackOnConnect(std::function<void(void)> callback) {
    on_connect = callback;
}

void SerialCommunication::RegisterCallbackOnClose(std::function<void(void)> callback) {
    on_close = callback;
}

void SerialCommunication::RegisterCallbackOnReceive(std::function<void(void*, size_t)> callback) {
    on_receive = callback;
}

void SerialCommunication::RegisterCallbackOnDisconnect(std::function<void(void)> callback) {
    on_disconnect = callback;
}

void SerialCommunication::RegisterCallbackOnReconnect(std::function<void(void)> callback) {
    on_reconnect = callback;
}

void SerialCommunication::RegisterCallbackOnTimeout(std::function<void(void)> callback) {
    on_timeout = callback;
}