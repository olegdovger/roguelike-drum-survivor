#include "GameState.h"
#include "Config.h"

void GameState::updateView(const sf::Vector2u& windowSize) {
    // Fixed logical size
    float logicalWidth = static_cast<float>(Config::get().getInt("Window.WIDTH", 1920));
    float logicalHeight = static_cast<float>(Config::get().getInt("Window.HEIGHT", 1080));
    const sf::Vector2f logicalSize(logicalWidth, logicalHeight);

    // Current window size
    float winWidth = static_cast<float>(windowSize.x);
    float winHeight = static_cast<float>(windowSize.y);

    // Compute viewport that preserves 4:3 aspect ratio
    float targetRatio = logicalSize.x / logicalSize.y;
    float winRatio = winWidth / winHeight;

    sf::Vector2f viewportPosition(0.f, 0.f);
    sf::Vector2f viewportSize(1.f, 1.f);  // default: full window

    if (winRatio > targetRatio) {
        // Window is wider → black bars on sides
        float width = targetRatio / winRatio;
        viewportPosition.x = (1.f - width) * 0.5f;
        viewportSize.x = width;
    } else if (winRatio < targetRatio) {
        // Window is taller → black bars on top/bottom
        float height = winRatio / targetRatio;
        viewportPosition.y = (1.f - height) * 0.5f;
        viewportSize.y = height;
    }

    // Create and set view
    currentView.setSize(logicalSize);
    currentView.setCenter(logicalSize / 2.f);
    currentView.setViewport(sf::FloatRect(viewportPosition, viewportSize));
}
