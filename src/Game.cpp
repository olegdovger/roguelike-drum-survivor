#include <stdexcept>
#include <string>

#include "GameState.h"
#include "Config.h"
#include "../engine/ECS/Components.h"
#include "Game.h"
#include "../engine/Events.h"


Game::Game()
    : window(sf::VideoMode({
                 static_cast<unsigned int>(Config::get().getInt("Window.WIDTH", 1920)),
                 static_cast<unsigned int>(Config::get().getInt("Window.HEIGHT", 1080))
             }),
             Config::get().getString("Window.TITLE", "Roguelike Drum Survivor")) {
  if (!window.isOpen()) {
    throw std::runtime_error("Failed to create window");
  }

  // Ensure config is loaded initially
  Config::get().update();

  window.setFramerateLimit(Config::get().getInt("Window.TARGET_FPS", 60));

  // Initialize view with letterboxing for initial window size
  GAME_STATE.updateView(window.getSize());
  window.setView(GAME_STATE.currentView);

  inputSystem = std::make_shared<InputSystem>();
  movementSystem = std::make_shared<MovementSystem>();
  renderSystem = std::make_shared<RenderSystem>();
  resizeSystem = std::make_shared<ResizeSystem>();
  configSystem = std::make_shared<ConfigSystem>();

  renderSystem->setWindow(&window);
  resizeSystem->setWindow(&window);
  inputSystem->setWindow(&window);

  ecs.addSystem(inputSystem);
  ecs.addSystem(movementSystem);
  ecs.addSystem(resizeSystem);
  ecs.addSystem(renderSystem);
  ecs.addSystem(configSystem);

  initialize();
}

void Game::run() {
  while (window.isOpen()) {
    float deltaTime = clock.restart().asSeconds();

    Config::get().update();

    handleEvents();
    update(deltaTime);
    render();

    float targetFrameTime = 1.0f / Config::get().getInt("Window.TARGET_FPS", 60);
    float elapsed = clock.getElapsedTime().asSeconds();
    if (elapsed < targetFrameTime) {
      sf::sleep(sf::seconds(targetFrameTime - elapsed));
    }
  }
}

void Game::handleEvents() {
  while (const auto event = window.pollEvent()) {
    switch (getEventType(*event)) {
    case EventType::Closed:
      window.close();
      break;
    case EventType::Resized:
      break;
    case EventType::KeyPressed:
      break;
    case EventType::Unknown:
    default:
      break;
    }
  }
}

void Game::update(float deltaTime) {
  configSystem->update(deltaTime, ecs); // Run config updates first
  inputSystem->update(deltaTime, ecs);
  movementSystem->update(deltaTime, ecs);
  resizeSystem->update(deltaTime, ecs);
}

void Game::render() {
  sf::Color clearColor(
      static_cast<std::uint8_t>(Config::get().getInt("Window.CLEAR_COLOR_R", 249)),
      static_cast<std::uint8_t>(Config::get().getInt("Window.CLEAR_COLOR_G", 163)),
      static_cast<std::uint8_t>(Config::get().getInt("Window.CLEAR_COLOR_B", 27))
  );
  window.clear(clearColor);
  renderSystem->update(0.0f, ecs);
  window.display();
}

void Game::initialize() {
  player = ecs.createEntity();

  float playerSize = Config::get().getFloat("Player.SIZE", 64.0f);
  float startX = (Config::get().getInt("Window.WIDTH", 1920) - playerSize) / 2.f;
  float startY = (Config::get().getInt("Window.HEIGHT", 1080) - playerSize) / 2.f;

  ecs.addComponent<PositionComponent>(player,
    std::make_shared<PositionComponent>(startX, startY));

  ecs.addComponent<VelocityComponent>(player,
    std::make_shared<VelocityComponent>(0.0f, 0.0f));

  sf::Color playerColor(
      static_cast<std::uint8_t>(Config::get().getInt("Player.COLOR_R", 0)),
      static_cast<std::uint8_t>(Config::get().getInt("Player.COLOR_G", 0)),
      static_cast<std::uint8_t>(Config::get().getInt("Player.COLOR_B", 0))
  );

  ecs.addComponent<ShapeComponent>(player,
    std::make_shared<ShapeComponent>(
      ShapeComponent::Type::Rectangle,
      playerColor));
  auto shape = ecs.getComponent<ShapeComponent>(player);
  if (shape) {
    shape->rectangle.width = playerSize;
    shape->rectangle.height = playerSize;
  }

  ecs.addComponent<InputComponent>(player,
    std::make_shared<InputComponent>());
  auto input = ecs.getComponent<InputComponent>(player);
  if (input) {
    input->moveSpeed = Config::get().getFloat("Player.MOVE_SPEED", 800.0f);
  }

  ecs.addComponent<ColliderComponent>(player,
    std::make_shared<ColliderComponent>(
      ColliderComponent::Type::Platform,
      playerSize,
      playerSize));
}

Game::~Game() {
}
