#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "../engine/ECS/ECSManager.h"
#include "../engine/ECS/Systems/InputSystem.h"
#include "../engine/ECS/Systems/MovementSystem.h"
#include "../engine/ECS/Systems/RenderSystem.h"
#include "../engine/ECS/Systems/ResizeSystem.h"
#include "../engine/ECS/Systems/ConfigSystem.h"

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
    std::shared_ptr<ConfigSystem> configSystem;

    Entity player;


    sf::Clock clock;
};
