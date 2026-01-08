#pragma once

#include <SFML/Graphics.hpp>

#include "../System.h"

class RenderSystem : public System
{
public:
    void setWindow(sf::RenderWindow* window);
    void update(float deltaTime, ECSManager& ecs) override;

private:
    sf::RenderWindow* window = nullptr;
};

