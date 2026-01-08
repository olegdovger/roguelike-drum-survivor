#pragma once

class ECSManager;

class System
{
public:
    virtual ~System() = default;
    virtual void update(float deltaTime, ECSManager& ecs) = 0;
};
