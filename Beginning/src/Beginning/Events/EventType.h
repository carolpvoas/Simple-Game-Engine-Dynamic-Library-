#pragma once

#include <unordered_map>
#include <string>
#include <variant>
//#include "EventType.h"

namespace Beginning {

enum class EventType {
    None = 0,
    Collision,
    Overlap
};

class Event {

public:
   
    virtual ~Event() = default;  // Virtual destructor for polymorphic base class
    virtual EventType getEventType() const = 0;  // Pure virtual function for type identification
};

}