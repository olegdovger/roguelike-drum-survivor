#pragma once

#include <SFML/Graphics.hpp>
#include "ECS/ECSManager.h"
#include "ECS/Systems/InputSystem.h"
#include "ECS/Systems/MovementSystem.h"
#include "ECS/Systems/RenderSystem.h"
#include "ECS/Systems/ResizeSystem.h"
#include <memory>

class Game
{
public:
    Game();
    ~Game();

    void run();

    Entity getPlayer() const { return player; }
    ECSManager& getECS() { return ecs; }
    const ECSManager& getECS() const { return ecs; }

private:
    void handleEvents();
    void update(float deltaTime);
    void render();
    void initialize();

    sf::RenderWindow window;
    ECSManager ecs;

    std::shared_ptr<InputSystem> inputSystem;
    std::shared_ptr<MovementSystem> movementSystem;
    std::shared_ptr<RenderSystem> renderSystem;
    std::shared_ptr<ResizeSystem> resizeSystem;

    Entity player;

    sf::Clock clock;
};
