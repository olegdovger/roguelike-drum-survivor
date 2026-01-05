#include "ResizeSystem.h"
#include "../ECSManager.h"
#include "../../GameState.h"
#include <SFML/Graphics.hpp>

void ResizeSystem::setWindow(sf::RenderWindow* win)
{
    window = win;
    if (window)
    {
        lastWidth = window->getSize().x;
        lastHeight = window->getSize().y;
    }
}



void ResizeSystem::update(float deltaTime, ECSManager& ecs)
{
    if (!window) return;

    unsigned int currentWidth = window->getSize().x;
    unsigned int currentHeight = window->getSize().y;

    if (currentWidth != lastWidth || currentHeight != lastHeight)
    {
        // Update view with letterboxing for new window size
        GAME_STATE.updateView(window->getSize());
        window->setView(GAME_STATE.currentView);
        lastWidth = currentWidth;
        lastHeight = currentHeight;
    }
}
