#pragma once
#include "Object.h"
#include "../Components/PawnMovementComponent.h"

//CLASS DEFINITION: It's a subclass of Object. It can be posseseed and receive input from a controller. It's a more basic class for simpler movement objects (veheicules, etc)

namespace Beginning {

    class Pawn : public Object {
    public:
        

        // Update method to be overridden
        virtual void OnUpdate(float deltaTime) override;


    protected:
        Pawn();
        virtual ~Pawn();
    };
}

