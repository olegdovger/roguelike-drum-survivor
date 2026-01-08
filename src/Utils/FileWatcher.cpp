#include "FileWatcher.h"
#include "LPP_API_x64_CPP.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

bool FileWatcher::shouldEnableFileWatching() {
    // Check for development environment indicators
    std::error_code ec;
    
    // Look for common development files/directories
    bool hasDevFiles = 
        fs::exists(".git", ec) ||
        fs::exists(".vscode", ec) ||
        fs::exists("CMakeLists.txt", ec) ||
        fs::exists("src", ec);
        
    return hasDevFiles;
}

FileWatcher::FileWatcher(const std::wstring& watchPath, lpp::LppDefaultAgent* agent, int timeoutMs)
    : m_watchPath(watchPath)
    , m_agent(agent)
    , m_timeoutMs(timeoutMs)
    , m_running(true)
    , m_reloadScheduled(false)
{
    m_watchThread = std::thread(&FileWatcher::watchLoop, this);
}

FileWatcher::~FileWatcher()
{
    m_running = false;
    if (m_watchThread.joinable())
    {
        m_watchThread.join();
    }
}

void FileWatcher::watchLoop()
{
    scanFiles();

    while (m_running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (hasFileChanged())
        {
            if (!m_reloadScheduled)
            {
                m_reloadScheduled = true;
                m_lastChangeTime = std::chrono::steady_clock::now();
            }
            else
            {
                auto now = std::chrono::steady_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastChangeTime).count();

                if (elapsed >= m_timeoutMs)
                {
                    if (m_agent && lpp::LppIsValidDefaultAgent(m_agent))
                    {
                        std::wcout << L"FileWatcher: Triggering Live++ hot-reload..." << std::endl;
                        m_agent->ScheduleReload();
                    }
                    m_reloadScheduled = false;
                    scanFiles();
                }
            }
        }
    }
}

void FileWatcher::scanFiles()
{
    m_fileModTimes.clear();
    
    try
    {
        // Convert to absolute path to handle relative paths consistently
        fs::path absolutePath = fs::absolute(m_watchPath);
        
        if (fs::exists(absolutePath) && fs::is_directory(absolutePath))
        {
            for (const auto& entry : fs::recursive_directory_iterator(absolutePath))
            {
                if (entry.is_regular_file())
                {
                    auto extension = entry.path().extension();
                    if (extension == ".cpp" || extension == ".h" || extension == ".hpp")
                    {
                        m_fileModTimes[entry.path()] = fs::last_write_time(entry.path());
                    }
                }
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "FileWatcher: Error scanning files - " << e.what() << std::endl;
    }
}

bool FileWatcher::hasFileChanged()
{
    try
    {
        // Convert to absolute path to handle relative paths consistently
        fs::path absolutePath = fs::absolute(m_watchPath);
        
        if (fs::exists(absolutePath) && fs::is_directory(absolutePath))
        {
            for (const auto& entry : fs::recursive_directory_iterator(absolutePath))
            {
                if (entry.is_regular_file())
                {
                    auto extension = entry.path().extension();
                    if (extension == ".cpp" || extension == ".h" || extension == ".hpp")
                    {
                        auto it = m_fileModTimes.find(entry.path());
                        auto currentModTime = fs::last_write_time(entry.path());
                        
                        if (it == m_fileModTimes.end() || it->second != currentModTime)
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "FileWatcher: Error checking for file changes - " << e.what() << std::endl;
    }
    return false;
}