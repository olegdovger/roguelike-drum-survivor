#pragma once

#include <SFML/Window/Event.hpp>

enum class EventType { Closed, Resized, KeyPressed, Unknown };

struct EventVisitor {
    EventType operator()(const sf::Event::Closed&) const {
        return EventType::Closed;
    }

    EventType operator()(const sf::Event::Resized&) const {
        return EventType::Resized;
    }

    EventType operator()(const sf::Event::KeyPressed&) const {
        return EventType::KeyPressed;
    }

    template <typename T>
    EventType operator()(const T&) const {
        return EventType::Unknown;
    }
};

inline EventType getEventType(const sf::Event &event) {
  return event.visit(EventVisitor{});
}
