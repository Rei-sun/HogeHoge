#pragma once

#include <json.hpp>
#include <string>
#include <fstream>
#include <iostream>

#ifndef HOGE_ROOT
#define HOGE_ROOT "This is a dummy. The real one is defined by CMake!"
#endif

namespace HogeGen2 {

struct ConfigFile {
    using json = nlohmann::json;
    std::string target_device_name;
    uint32_t baudrate;
    bool use_gui;
    uint32_t gui_server_port;

    void Load(json j) {
        if (!j[key_HogeGen2][key_target_device_name].is_null() && j[key_HogeGen2][key_target_device_name].is_string()) {
            target_device_name = j[key_HogeGen2][key_target_device_name].get<std::string>();
        } else {
            target_device_name = "/dev/ttyACM0";
        }

        if (!j[key_HogeGen2][key_baudrate].is_null() && j[key_HogeGen2][key_baudrate].is_number_unsigned()) {
            baudrate = j[key_HogeGen2][key_baudrate].get<uint32_t>();
        } else {
            baudrate = 2000000;
        }

        if (!j[key_HogeGen2][key_use_gui].is_null() && j[key_HogeGen2][key_use_gui].is_boolean()) {
            use_gui = j[key_HogeGen2][key_use_gui].get<bool>();
        } else {
            use_gui = false;
        }

        if (!j[key_HogeGen2][key_gui_server_port].is_null() && j[key_HogeGen2][key_gui_server_port].is_number_unsigned()) {
            gui_server_port = j[key_HogeGen2][key_gui_server_port].get<uint32_t>();
        } else {
            gui_server_port = 20000;   
        }
    }
private:
    static inline const std::string key_HogeGen2 = std::string("HogeGen2");
    static inline const std::string key_target_device_name = std::string("target_device_name");
    static inline const std::string key_baudrate = std::string("baudrate");
    static inline const std::string key_use_gui = std::string("use_gui");
    static inline const std::string key_gui_server_port = std::string("gui_server_port");
};

class ConfigFileLoader {
    static inline const std::string ERROR_STR = std::string("error");
    static inline bool is_success = false;
    
    using json = nlohmann::json;
    static json LoadJsonFile(std::string filepath) {
        std::fstream ifs(filepath);
        json j; ifs >> j; ifs.close();
        return j;
    }
    static std::string GetConfigFileName() {
        try {
            json j = LoadJsonFile(HOGE_ROOT_PATH + SYSTEM_CONFIG_FILENAME);
            if (!j["LoadConfig"].is_null() && j["LoadConfig"].is_string()) {
                return j["LoadConfig"].get<std::string>();
            }
        } catch (json::exception e) {
            std::cout << e.what() << "\n";
        }

        return ERROR_STR;
    }

    static bool LoadConfigFile(std::string filename, ConfigFile &config) {
        try {
            json j = LoadJsonFile(HOGE_ROOT_PATH + filename);
            config.Load(j);
        } catch (json::exception e) {
            std::cout << e.what() << "\n";
            return false;
        }
        return true;
    }
public:
    static inline const std::string HOGE_ROOT_PATH = std::string(HOGE_ROOT);
    static inline const std::string SYSTEM_CONFIG_FILENAME = std::string("system.json");
    static inline std::string config_filename = std::string("undefined");
    static inline ConfigFile config;

    static void LoadConfig() {
        // read config file name from system config file name.
        config_filename = GetConfigFileName();
        if (config_filename == ERROR_STR) return;

        // read config file
        std::cout << "Config Loaded: " << config_filename << "\n";
        is_success = LoadConfigFile(config_filename,  config);
    }

    static bool IsConfigLoaded() {
        return is_success;
    }
};

}
