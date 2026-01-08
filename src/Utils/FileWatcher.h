#pragma once

#include <filesystem>
#include <chrono>
#include <map>
#include <thread>
#include <atomic>
#include <string>

// Forward declaration to avoid including Live++ header
namespace lpp
{
    struct LppDefaultAgent;
}

namespace fs = std::filesystem;

/**
 * @brief Watches for file system changes and triggers Live++ hot-reload
 * 
 * The FileWatcher monitors a directory for changes to source files
 * (.cpp, .h, .hpp) and automatically schedules a Live++ reload after
 * a configurable timeout. This provides automatic hot-reload without
 * requiring manual key presses.
 */
class FileWatcher
{
public:
    /**
     * @brief Construct a new File Watcher object
     * 
     * @param watchPath The directory path to watch for changes
     * @param agent Pointer to the Live++ agent (can be nullptr)
     * @param timeoutMs The timeout in milliseconds to wait after last change before triggering reload
     */
    FileWatcher(const std::wstring& watchPath, lpp::LppDefaultAgent* agent, int timeoutMs = 500);

    /**
     * @brief Destroy the File Watcher object
     * 
     * Stops the watching thread and joins it safely
     */
    ~FileWatcher();

    // Disable copy and move to prevent issues with thread and atomic members
    FileWatcher(const FileWatcher&) = delete;
    FileWatcher& operator=(const FileWatcher&) = delete;
    FileWatcher(FileWatcher&&) = delete;
    FileWatcher& operator=(FileWatcher&&) = delete;

    /**
     * @brief Check if file watching should be enabled (development mode)
     * 
     * @return true if file watching should be enabled, false for production
     */
    static bool shouldEnableFileWatching();

private:
    /**
     * @brief Main watch loop that runs on a separate thread
     */
    void watchLoop();

    /**
     * @brief Scan all files in the watch directory and store modification times
     */
    void scanFiles();

    /**
     * @brief Check if any watched file has been modified
     * 
     * @return true if a file change was detected, false otherwise
     */
    bool hasFileChanged();

private:
    std::wstring m_watchPath;
    lpp::LppDefaultAgent* m_agent;
    int m_timeoutMs;
    std::map<fs::path, fs::file_time_type> m_fileModTimes;
    std::thread m_watchThread;
    std::atomic<bool> m_running;
    std::atomic<bool> m_reloadScheduled;
    std::chrono::steady_clock::time_point m_lastChangeTime;
};