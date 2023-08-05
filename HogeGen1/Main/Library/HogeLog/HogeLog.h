#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <stdarg.h>
#include <chrono>
#include <sstream>
#include <iomanip>

#define __NANO_TIME             std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch())
#define __SEC_TIME              std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch())

class LogConsole{
private:    
    static std::string top(std::string color, std::string level, long long sec, long long nano){
        std::stringstream ss;
        ss << color << "[" << std::setw(5) << level << "] [" << sec << "." << std::setw(9) << std::setfill('0') << nano << "]";
        return ss.str();
    }
    static std::string end(){
        return "\n\x1b[39m";
    }
    static void Print(int level, std::string text){
        switch(level){
            case LOG_LEVEL_DEBUG:
                std::cout << top("\x1b[39m", "Debug", __SEC_TIME.count(), (__NANO_TIME - __SEC_TIME).count()) << text << end();
                break;
            case LOG_LEVEL_INFO:
                std::cout << top("\x1b[39m", "Info", __SEC_TIME.count(), (__NANO_TIME - __SEC_TIME).count()) << text << end();
                break;
            case LOG_LEVEL_WARN:
                std::cout << top("\x1b[33m", "Warn", __SEC_TIME.count(), (__NANO_TIME - __SEC_TIME).count()) << text << end();
                break;
            case LOG_LEVEL_ERROR:
                std::cout << top("\x1b[31m", "Error", __SEC_TIME.count(), (__NANO_TIME - __SEC_TIME).count()) << text << end();
                break;
            case LOG_LEVEL_FATAL:
                std::cout << top("\x1b[31m", "Fatal", __SEC_TIME.count(), (__NANO_TIME - __SEC_TIME).count()) << text << end();
                break;
        }
    }
    static std::string log(const char* format, va_list ap){
        char buffer[128] = {0};
        vsnprintf(buffer, 128, format, ap);
        return std::string(buffer);
    }
public:
    enum {
        LOG_LEVEL_DEBUG,
        LOG_LEVEL_INFO,
        LOG_LEVEL_WARN,
        LOG_LEVEL_ERROR,
        LOG_LEVEL_FATAL
    };
    static void Info(const char* format, ...){
        va_list ap;
        va_start( ap, format);
        Print(LOG_LEVEL_INFO, log(format, ap));
        va_end(ap);
    }
    static void Debug(const char* format, ...){
        va_list ap;
        va_start( ap, format );
        Print(LOG_LEVEL_DEBUG, log(format, ap));
        va_end(ap);
    }
    static void Warn(const char* format, ...){
        va_list ap;
        va_start( ap, format );
        Print(LOG_LEVEL_WARN, log(format, ap));
        va_end(ap);
    }
    static void Error(const char* format, ...){
        va_list ap;
        va_start( ap, format );
        Print(LOG_LEVEL_ERROR, log(format, ap));
        va_end(ap);
    }
    static void Fatal(const char* format, ...){
        va_list ap;
        va_start( ap, format );
        Print(LOG_LEVEL_FATAL, log(format, ap));
        va_end(ap);
    }
};