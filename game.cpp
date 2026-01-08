#include <Windows.h>

#include "src/Game.h"
#include "src/Utils/FileWatcher.h"
#include "LPP_API_x64_CPP.h"

int main()
{
    // Initialize Live++
    lpp::LppDefaultAgent lppAgent = lpp::LppCreateDefaultAgent(nullptr, L"D:/Projects/SchoolXYZ/roguelike-samurai/ThirdParty/LivePP/LivePP");

    if (lpp::LppIsValidDefaultAgent(&lppAgent))
    {
        lppAgent.EnableModule(lpp::LppGetCurrentModulePath(), lpp::LPP_MODULES_OPTION_ALL_IMPORT_MODULES, nullptr, nullptr);
    }

    // Start file watcher for auto hot-reload (500ms timeout)
    FileWatcher watcher(L"D:/Projects/SchoolXYZ/roguelike-samurai", &lppAgent, 500);

    Game game;
    game.run();

    lpp::LppDestroyDefaultAgent(&lppAgent);
    return 0;
}
