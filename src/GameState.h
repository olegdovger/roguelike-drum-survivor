#pragma once

#include <SFML/Graphics.hpp>

class GameState
{
private:
    GameState() = default;

public:
    static GameState& Instance() {
      static GameState instance;
      return instance;
    }

    // Window settings
    const unsigned int WINDOW_WIDTH = 1920;
    const unsigned int WINDOW_HEIGHT = 1080;
    const char* WINDOW_TITLE = "Roguelike Drum Survivor";

    // Frame rate settings
    const unsigned int TARGET_FPS = 60;
    const float FRAME_TIME = 1.0f / TARGET_FPS;

    // Player settings
    const float PLAYER_SIZE = 32.0f;

    const float PLAYER_START_X = (WINDOW_WIDTH - PLAYER_SIZE) / 2.f;
    const float PLAYER_START_Y = (WINDOW_HEIGHT - PLAYER_SIZE) / 2.f;
    const float PLAYER_MOVE_SPEED = 400.0f;

    // View management
    sf::View currentView;

    void updateView(const sf::Vector2u& windowSize) {
        // Fixed logical size
        const sf::Vector2f logicalSize(static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT));

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

        // Debug output

        // Create and set view
        currentView.setSize(logicalSize);
        currentView.setCenter(logicalSize / 2.f);
        currentView.setViewport(sf::FloatRect(viewportPosition, viewportSize));
    }
};

#define GAME_STATE GameState::Instance()
