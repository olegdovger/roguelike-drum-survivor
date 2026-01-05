#include <stdexcept>

#include "GameState.h"
#include "ECS/Components.h"
#include "Game.h"
#include "Events.h"
#include "SFML/System/Vector2.hpp"

Game::Game()
    : window(sf::VideoMode({GAME_STATE.WINDOW_WIDTH, GAME_STATE.WINDOW_HEIGHT}),
             GAME_STATE.WINDOW_TITLE) {
  if (!window.isOpen()) {
    throw std::runtime_error("Failed to create window");
  }

  window.setFramerateLimit(GAME_STATE.TARGET_FPS);

  // Initialize view with letterboxing for initial window size
  GAME_STATE.updateView(window.getSize());
  window.setView(GAME_STATE.currentView);

  inputSystem = std::make_shared<InputSystem>();
  movementSystem = std::make_shared<MovementSystem>();
  renderSystem = std::make_shared<RenderSystem>();
  resizeSystem = std::make_shared<ResizeSystem>();

  renderSystem->setWindow(&window);
  resizeSystem->setWindow(&window);

  ecs.addSystem(inputSystem);
  ecs.addSystem(movementSystem);
  ecs.addSystem(resizeSystem);
  ecs.addSystem(renderSystem);

  initialize();
}

void Game::run() {
  while (window.isOpen()) {
    float deltaTime = clock.restart().asSeconds();
    handleEvents();
    update(deltaTime);
    render();

    float elapsed = clock.getElapsedTime().asSeconds();
    if (elapsed < GAME_STATE.FRAME_TIME) {
      sf::sleep(sf::seconds(GAME_STATE.FRAME_TIME - elapsed));
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
  inputSystem->update(deltaTime, ecs);
  movementSystem->update(deltaTime, ecs);
  resizeSystem->update(deltaTime, ecs);
}

void Game::render() {
  window.clear(sf::Color(249, 163, 27));
  renderSystem->update(0.0f, ecs);
  window.display();
}

void Game::initialize() {
  player = ecs.createEntity();

  ecs.addComponent<PositionComponent>(player,
    std::make_shared<PositionComponent>(
      GAME_STATE.PLAYER_START_X,
      GAME_STATE.PLAYER_START_Y));

  ecs.addComponent<VelocityComponent>(player,
    std::make_shared<VelocityComponent>(0.0f, 0.0f));

  ecs.addComponent<ShapeComponent>(player,
    std::make_shared<ShapeComponent>(
      ShapeComponent::Type::Rectangle,
      sf::Color::Black));
  auto shape = ecs.getComponent<ShapeComponent>(player);
  if (shape) {
    shape->rectangle.width = GAME_STATE.PLAYER_SIZE;
    shape->rectangle.height = GAME_STATE.PLAYER_SIZE;
  }

  ecs.addComponent<InputComponent>(player,
    std::make_shared<InputComponent>());
  auto input = ecs.getComponent<InputComponent>(player);
  if (input) {
    input->moveSpeed = GAME_STATE.PLAYER_MOVE_SPEED;
  }

  ecs.addComponent<ColliderComponent>(player,
    std::make_shared<ColliderComponent>(
      ColliderComponent::Type::Platform,
      GAME_STATE.PLAYER_SIZE,
      GAME_STATE.PLAYER_SIZE));
}

Game::~Game() {
}
