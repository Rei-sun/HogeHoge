// ESC 参考
// https://www.serendip.ws/archives/4635

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdarg.h>
#include <iomanip>
#include <chrono>
#include <unistd.h>
#include <vector>

namespace HogeGen2 {

class MessageOutputter {
    enum class MessageLevel : int {
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        Undefined
    };

    static inline std::vector<std::string> level_str {
        "DEBUG",
        "INFO",
        "WARN",
        "ERROR",
        "FATAL"
    };

    static std::ostream& endesc(std::ostream& ros) { return ros << "\x1b[39m"; }
    static std::ostream& setesc_black(std::ostream& ros) { return ros << "\x1b[30m"; }
    static std::ostream& setesc_red(std::ostream& ros) { return ros << "\x1b[31m"; }
    static std::ostream& setesc_green(std::ostream& ros) { return ros << "\x1b[32m"; }
    static std::ostream& setesc_yellow(std::ostream& ros) { return ros << "\x1b[33m"; }
    static std::ostream& setesc_blue(std::ostream& ros) { return ros << "\x1b[34m"; }
    static std::ostream& setesc_magenta(std::ostream& ros) { return ros << "\x1b[35m"; }
    static std::ostream& setesc_cyan(std::ostream& ros) { return ros << "\x1b[36m"; }
    static std::ostream& setesc_white(std::ostream& ros) { return ros << "\x1b[37m"; }

    static std::chrono::nanoseconds nanoseconds(std::chrono::_V2::system_clock::duration duration) {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
    }

    static std::chrono::seconds seconds(std::chrono::_V2::system_clock::duration duration) {
        return std::chrono::duration_cast<std::chrono::seconds>(duration);
    }

    static std::string GetFormattedTime(std::chrono::_V2::system_clock::time_point time){
        auto now_duration = time.time_since_epoch();
        auto sec = seconds(now_duration);
        auto nano = nanoseconds(now_duration);
        std::stringstream ss;
        ss << "[" << sec.count() << "." << std::setw(9) << std::setfill('0') << (nano - sec).count() << "]";
        return ss.str();
    }

    static std::string GetProcessID() {
        std::stringstream ss;
        ss << "[" << std::to_string(getpid()) << "]";
        return ss.str();
    }

    static std::string GetLevel(int level) {
        std::stringstream ss;
        level = (int)MessageLevel::Undefined <= level ? (int)MessageLevel::DEBUG : level;
        ss << "[" << std::setw(5) << level_str[level] << "]";
        return ss.str();
    }

    static std::string AddLevelColor(std::string str, int level) {
        std::stringstream ss;
        switch (level) {
        case (int)MessageLevel::DEBUG:
            ss << setesc_cyan << str << endesc;
            return ss.str(); 
        case (int)MessageLevel::INFO:
            ss << setesc_white << str << endesc;
            return ss.str();
            break;
        case (int)MessageLevel::WARN:
            ss << setesc_yellow << str << endesc;
            return ss.str();
            break;
        case (int)MessageLevel::ERROR:
            ss << setesc_red << str << endesc;
            return ss.str();
            break;
        case (int)MessageLevel::FATAL:
            ss << setesc_red << str << endesc;
            return ss.str();
            break;
        default:
            ss << setesc_white << str << endesc;
            return ss.str();
            break;
        }
    }

    std::ofstream *ofs;
    std::ostream *output_stream;
    char buffer[512];

    void Message(std::string message, int level = 0) {
        if (output_stream == nullptr) return;
        
        std::stringstream ss;
        
        ss << GetProcessID() << GetLevel(level) << GetFormattedTime(std::chrono::system_clock::now()) << ": " << message << "\n";
        
        (*output_stream) << ss.str();
        
        if (ofs != nullptr && !ofs->fail()) {
            std::cout << AddLevelColor(ss.str(), level);
        }
    }

public:
    MessageOutputter() {
        output_stream = &std::cout;
    }

    void Init(std::string output_path) {
        ofs = new std::ofstream(output_path);
        output_stream = ofs->fail() ? &std::cout : ofs;
    }

    void Init(std::ostream *output) {
        ofs = nullptr;
        output_stream = output;
    }

    virtual ~MessageOutputter() {
        if (ofs != nullptr && !ofs->fail()) {
            ofs->close();
        }
    }

    void DebugMessage(const char* format, ...) {
        va_list ap;
        va_start(ap, format);
        vsnprintf(buffer, 512, format, ap);
        Message(buffer, (int)MessageLevel::DEBUG);
        va_end(ap);
    }

    void InfoMessage(const char* format, ...) {
        va_list ap;
        va_start(ap, format);
        vsnprintf(buffer, 512, format, ap);
        Message(buffer, (int)MessageLevel::INFO);
        va_end(ap);
    }

    void WarnMessage(const char* format, ...) {
        va_list ap;
        va_start(ap, format);
        vsnprintf(buffer, 512, format, ap);
        Message(buffer, (int)MessageLevel::WARN);
        va_end(ap);
    }

    void ErrorMessage(const char* format, ...) {
        va_list ap;
        va_start(ap, format);
        vsnprintf(buffer, 512, format, ap);
        Message(buffer, (int)MessageLevel::ERROR);
        va_end(ap);
    }

    void FatalMessage(const char* format, ...) {
        va_list ap;
        va_start(ap, format);
        vsnprintf(buffer, 512, format, ap);
        Message(buffer, (int)MessageLevel::FATAL);
        va_end(ap);
    }
};

inline MessageOutputter log_output;

}
