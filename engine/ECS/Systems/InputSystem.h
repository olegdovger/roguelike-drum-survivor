#pragma once

#include <SFML/Window/Keyboard.hpp>

#include "../System.h"

class InputSystem : public System
{
public:
    void update(float deltaTime, ECSManager& ecs) override;
};

