#include "MovementSystem.h"
#include "../ECSManager.h"
#include "../Components.h"
#include <map>
#include <algorithm>

// Acceleration/deceleration rates
constexpr float ACCELERATION_RATE = 2.0f;  // How fast to reach target direction (per second)
constexpr float DECELERATION_RATE = 3.0f;  // How fast to return to 0 when no input

// Track current direction values for smooth transitions per entity
static std::map<Entity, sf::Vector2f> g_currentDirections;

void MovementSystem::update(float deltaTime, ECSManager& ecs)
{
    auto entities = ecs.getEntitiesWithComponent<PositionComponent>();

    for (Entity entity : entities)
    {
        auto position = ecs.getComponent<PositionComponent>(entity);
        auto velocity = ecs.getComponent<VelocityComponent>(entity);
        auto input = ecs.getComponent<InputComponent>(entity);

        if (!position) continue;

        if (input && velocity)
        {
            // Determine target direction based on input
            float targetHorizontalDirection = 0.0f;
            float targetVerticalDirection = 0.0f;

            if (input->leftPressed)
                targetHorizontalDirection = -0.5f;
            else if (input->rightPressed)
                targetHorizontalDirection = 0.5f;

            if (input->upPressed)
                targetVerticalDirection = -0.5f;
            else if (input->downPressed)
                targetVerticalDirection = 0.5f;

            // Get or create current direction for this entity
            sf::Vector2f& currentDirection = g_currentDirections[entity];

            // Smoothly interpolate current direction towards target direction
            float lerpRate = (targetHorizontalDirection == 0.0f) ? DECELERATION_RATE : ACCELERATION_RATE;
            currentDirection.x = currentDirection.x + (targetHorizontalDirection - currentDirection.x) * lerpRate * deltaTime;

            lerpRate = (targetVerticalDirection == 0.0f) ? DECELERATION_RATE : ACCELERATION_RATE;
            currentDirection.y = currentDirection.y + (targetVerticalDirection - currentDirection.y) * lerpRate * deltaTime;

            // Clamp values to valid range [-0.5f, 0.5f]
            currentDirection.x = std::max(-0.5f, std::min(0.5f, currentDirection.x));
            currentDirection.y = std::max(-0.5f, std::min(0.5f, currentDirection.y));

            // Apply velocity using the smoothed direction
            velocity->velocity.x = currentDirection.x * input->moveSpeed;
            velocity->velocity.y = currentDirection.y * input->moveSpeed;
        }

        if (velocity)
        {
            position->position += velocity->velocity * deltaTime;
        }
    }
}
