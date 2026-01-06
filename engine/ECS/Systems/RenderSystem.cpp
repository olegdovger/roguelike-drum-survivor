#include "RenderSystem.h"
#include "../ECSManager.h"
#include "../Components.h"
#include "src/GameState.h"

void RenderSystem::setWindow(sf::RenderWindow* win)
{
    window = win;
}

void RenderSystem::update(float deltaTime, ECSManager& ecs)
{
    if (!window) return;

    
    window->setView(GAME_STATE.currentView);

    
    auto entities = ecs.getEntitiesWithComponent<PositionComponent>();

    for (Entity entity : entities)
    {
        auto position = ecs.getComponent<PositionComponent>(entity);
        auto shape = ecs.getComponent<ShapeComponent>(entity);

        if (!position || !shape) continue;

        if (shape->type == ShapeComponent::Type::Rectangle)
        {
            sf::RectangleShape rect;
            
            rect.setSize({shape->rectangle.width, shape->rectangle.height});
            rect.setPosition(position->position);
            rect.setFillColor(shape->color);



            window->draw(rect);


        }
        else if (shape->type == ShapeComponent::Type::Circle)
        {
            
            float radius = shape->circle.radius;
            sf::CircleShape circle(radius);
            circle.setPosition({position->position.x - radius,
                               position->position.y - radius});
            circle.setFillColor(shape->color);
            window->draw(circle);
        }
    }
}
