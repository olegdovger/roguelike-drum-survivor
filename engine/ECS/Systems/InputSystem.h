#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "../System.h"

class InputSystem : public System
{
public:
    void update(float deltaTime, ECSManager& ecs) override;
    void setWindow(sf::RenderWindow* win) { window = win; }

private:
    sf::RenderWindow* window = nullptr;
};

