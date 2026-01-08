#pragma once

#include "../ECSManager.h"
#include <cstddef>

class ConfigSystem : public System
{
public:
    ConfigSystem();
    ~ConfigSystem() override;
    void update(float deltaTime, ECSManager& ecs) override;

private:
    bool m_dirty = true; // Force initial update
    size_t m_callbackId = 0;
};
