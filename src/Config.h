#pragma once
#include <string>
#include <map>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <functional>

class Config {
public:
    static Config& get() {
        static Config instance;
        return instance;
    }

    Config() {
        load("config.ini");
    }

    size_t registerCallback(std::function<void()> callback) {
        size_t id = nextCallbackId++;
        callbacks[id] = callback;
        return id;
    }

    void unregisterCallback(size_t id) {
        callbacks.erase(id);
    }

    void update() {
        const std::string path = "config.ini";
        std::error_code ec;
        if (!std::filesystem::exists(path, ec)) return;

        auto currTime = std::filesystem::last_write_time(path, ec);
        if (currTime != lastTime) {
            load(path);
            lastTime = currTime;
            notifyObservers();
        }
    }

    float getFloat(const std::string& key, float defaultValue) {
        if (floatValues.find(key) != floatValues.end()) return floatValues[key];
        return defaultValue;
    }

    int getInt(const std::string& key, int defaultValue) {
        if (intValues.find(key) != intValues.end()) return intValues[key];
        return defaultValue;
    }

    std::string getString(const std::string& key, const std::string& defaultValue) {
        if (stringValues.find(key) != stringValues.end()) return stringValues[key];
        return defaultValue;
    }

private:
    std::filesystem::file_time_type lastTime;
    std::map<std::string, float> floatValues;
    std::map<std::string, int> intValues;
    std::map<std::string, std::string> stringValues;
    std::map<size_t, std::function<void()>> callbacks;
    size_t nextCallbackId = 0;

    void notifyObservers() {
        for (const auto& [id, callback] : callbacks) {
            if (callback) callback();
        }
    }

    void load(const std::string& path) {
        std::ifstream file(path);
        std::string line;
        std::string currentSection;

        while (std::getline(file, line)) {
            if (line.empty()) continue;
            
            // Trim leading/trailing whitespace including \r for Windows compatibility
            size_t first = line.find_first_not_of(" \t\r\n");
            if (first == std::string::npos) continue;
            size_t last = line.find_last_not_of(" \t\r\n");
            line = line.substr(first, (last - first + 1));

            if (line.front() == '[' && line.back() == ']') {
                currentSection = line.substr(1, line.size() - 2);
            } else {
                size_t delimiterPos = line.find('=');
                if (delimiterPos != std::string::npos) {
                    std::string keyPart = line.substr(0, delimiterPos);
                    std::string valuePart = line.substr(delimiterPos + 1);

                    // Trim key
                    size_t kFirst = keyPart.find_first_not_of(" \t");
                    size_t kLast = keyPart.find_last_not_of(" \t");
                    if (kFirst != std::string::npos) {
                        keyPart = keyPart.substr(kFirst, (kLast - kFirst + 1));
                    }

                    // Trim value
                    size_t vFirst = valuePart.find_first_not_of(" \t\r\n");
                    size_t vLast = valuePart.find_last_not_of(" \t\r\n");
                    if (vFirst != std::string::npos) {
                        valuePart = valuePart.substr(vFirst, (vLast - vFirst + 1));
                    }

                    std::string key = currentSection + "." + keyPart;
                    
                    try {
                        floatValues[key] = std::stof(valuePart);
                        intValues[key] = std::stoi(valuePart);
                    } catch (...) {}
                    stringValues[key] = valuePart;
                }
            }
        }
        std::cout << "Configuration reloaded" << std::endl;
    }
};
