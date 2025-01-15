#pragma once

#include "EventType.h"
#include "../Objects/Object.h"

namespace Beginning {

class CollisionEvent : public Event {

public:
    void* collider;  // Pointers to the objects involved in the collision
    void* collidee;

    CollisionEvent(void* collider, void* collidee)
        : collider(collider), collidee(collidee) {}

    EventType getEventType() const override {
        return EventType::Collision;  // Return the specific event type
    }
};

class OverlapEvent : public Event {

public:
    void* collider;  // Pointers to the objects involved in the collision
    void* collidee;

    OverlapEvent(void* collider, void* collidee)
        : collider(collider), collidee(collidee) {}

    EventType getEventType() const override {
        return EventType::Overlap;  // Return the specific event type
    }
};

}