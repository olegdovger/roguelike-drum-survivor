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

    sf::View currentView;

    void updateView(const sf::Vector2u& windowSize);
};

#define GAME_STATE GameState::Instance()
