#pragma once

#include <SFML/Window/Event.hpp>

enum class EventType { Closed, Resized, KeyPressed, Unknown };

inline EventType getEventType(const sf::Event &event) {
  if (event.is<sf::Event::Closed>())
    return EventType::Closed;
  if (event.is<sf::Event::Resized>())
    return EventType::Resized;
  if (event.is<sf::Event::KeyPressed>())
    return EventType::KeyPressed;
  return EventType::Unknown;
}