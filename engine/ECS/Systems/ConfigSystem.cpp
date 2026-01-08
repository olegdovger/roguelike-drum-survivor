#include "ConfigSystem.h"
#include "../Components.h"
#include "src/Config.h"
#include <iostream>
#include <cstdint>
#include <algorithm>

ConfigSystem::ConfigSystem() {
    m_callbackId = Config::get().registerCallback([this]() {
        m_dirty = true;
    });
}

ConfigSystem::~ConfigSystem() {
    Config::get().unregisterCallback(m_callbackId);
}

void ConfigSystem::update(float deltaTime, ECSManager& ecs) {
    if (!m_dirty) return;

    float playerSize = Config::get().getFloat("Player.SIZE", 64.0f);
    float playerSpeed = Config::get().getFloat("Player.MOVE_SPEED", 800.0f);

    int r = Config::get().getInt("Player.COLOR_R", 0);
    int g = Config::get().getInt("Player.COLOR_G", 0);
    int b = Config::get().getInt("Player.COLOR_B", 0);

    sf::Color playerColor(
        static_cast<std::uint8_t>(std::clamp(r, 0, 255)),
        static_cast<std::uint8_t>(std::clamp(g, 0, 255)),
        static_cast<std::uint8_t>(std::clamp(b, 0, 255))
    );

    auto inputEntities = ecs.getEntitiesWithComponent<InputComponent>();

    for (auto entity : inputEntities) {
        if (auto shape = ecs.getComponent<ShapeComponent>(entity)) {
            shape->rectangle.width = playerSize;
            shape->rectangle.height = playerSize;
            shape->color = playerColor;
        }

        if (auto collider = ecs.getComponent<ColliderComponent>(entity)) {
            collider->size = {playerSize, playerSize};
        }

        if (auto input = ecs.getComponent<InputComponent>(entity)) {
            input->moveSpeed = playerSpeed;
        }
    }

    std::cout << "ConfigSystem: Entities updated from config." << std::endl;
    m_dirty = false;
}
