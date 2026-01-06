#pragma once

#include <SFML/Graphics.hpp>

#include "../System.h"

class ResizeSystem : public System
{
public:
    void setWindow(sf::RenderWindow* window);
    void update(float deltaTime, ECSManager& ecs) override;

private:
    sf::RenderWindow* window = nullptr;
    unsigned int lastWidth = 0;
    unsigned int lastHeight = 0;
};
