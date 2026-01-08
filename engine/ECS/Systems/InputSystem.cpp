#include "InputSystem.h"
#include "../ECSManager.h"
#include "../Components.h"

void InputSystem::update(float deltaTime, ECSManager& ecs)
{

    if (window && !window->hasFocus())
        return;

    auto entities = ecs.getEntitiesWithComponent<InputComponent>();

    for (Entity entity : entities)
    {
        auto input = ecs.getComponent<InputComponent>(entity);
        if (!input) continue;

        input->leftPressed = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
                             sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left));
        input->rightPressed = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
                              sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right));
        input->upPressed = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
                            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up));
        input->downPressed = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) ||
                              sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down));
        input->actionPressed = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) ||
                                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter));
    }
}
