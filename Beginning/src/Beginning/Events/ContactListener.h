#pragma once

#include <box2d/box2d.h>
#include "EventHandler.h"
#include "CollisionEvent.h"

namespace Beginning {

    class ContactListener : public b2ContactListener {
    public:
        void BeginContact(b2Contact* contact) override;

        bool IsHit(void* hitter, void* hittee) {
        };

        ~ContactListener();

    };
}