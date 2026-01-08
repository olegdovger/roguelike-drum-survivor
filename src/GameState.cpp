#include "GameState.h"
#include "Config.h"

void GameState::updateView(const sf::Vector2u& windowSize) {
    float logicalWidth = static_cast<float>(Config::get().getInt("Window.WIDTH", 1920));
    float logicalHeight = static_cast<float>(Config::get().getInt("Window.HEIGHT", 1080));
    const sf::Vector2f logicalSize(logicalWidth, logicalHeight);

    float winWidth = static_cast<float>(windowSize.x);
    float winHeight = static_cast<float>(windowSize.y);

    float targetRatio = logicalSize.x / logicalSize.y;
    float winRatio = winWidth / winHeight;

    sf::Vector2f viewportPosition(0.f, 0.f);
    sf::Vector2f viewportSize(1.f, 1.f);

    if (winRatio > targetRatio) {
        float width = targetRatio / winRatio;
        viewportPosition.x = (1.f - width) * 0.5f;
        viewportSize.x = width;
    } else if (winRatio < targetRatio) {
        float height = winRatio / targetRatio;
        viewportPosition.y = (1.f - height) * 0.5f;
        viewportSize.y = height;
    }

    currentView.setSize(logicalSize);
    currentView.setCenter(logicalSize / 2.f);
    currentView.setViewport(sf::FloatRect(viewportPosition, viewportSize));
}
