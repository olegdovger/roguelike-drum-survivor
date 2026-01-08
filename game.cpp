#include <Windows.h>
#include <iostream>
#include <memory>

#include "src/Game.h"
#include "src/Utils/FileWatcher.h"
#include "LPP_API_x64_CPP.h"

std::wstring GetExecutableDirectory() {
    wchar_t path[MAX_PATH] = {0};
    if (GetModuleFileNameW(nullptr, path, MAX_PATH) > 0) {
        std::wstring fullPath(path);
        size_t lastSlash = fullPath.find_last_of(L"\/");
        if (lastSlash != std::wstring::npos) {
            return fullPath.substr(0, lastSlash + 1);
        }
    }
    return L"./";
}

int main()
{
    // Get executable directory for relative path resolution
    std::wstring exeDir = GetExecutableDirectory();
    std::wstring livePPPath = exeDir + L"ThirdParty\\LivePP\\LivePP";
    
    lpp::LppDefaultAgent lppAgent = lpp::LppCreateDefaultAgent(nullptr, livePPPath.c_str());

    if (lpp::LppIsValidDefaultAgent(&lppAgent))
    {
        lppAgent.EnableModule(lpp::LppGetCurrentModulePath(), lpp::LPP_MODULES_OPTION_ALL_IMPORT_MODULES, nullptr, nullptr);
    }

    std::unique_ptr<FileWatcher> fileWatcher;
    if (FileWatcher::shouldEnableFileWatching()) {
        fileWatcher = std::make_unique<FileWatcher>(exeDir, &lppAgent, 500);
        std::cout << "Development mode: File watching enabled" << std::endl;
    } else {
        std::cout << "Production mode: File watching disabled" << std::endl;
    }

    Game game;
    game.run();

    lpp::LppDestroyDefaultAgent(&lppAgent);
    return 0;
}
