#pragma once
#include "Pawn.h"
#include "ObjectManager.h"
//#include "PawnMovementComponent.h"  // Assuming you have or will create a movement component

//CLASS DEFINITION: It's a subclass of Object. It's designed to have a movement component and be controlled by a controller.

namespace Beginning {

    class Character : public Pawn {
    public:
        
        //std::shared_ptr<RenderComponent> RenderComponent;
        //std::shared_ptr<ColliderComponent> collider;
        std::shared_ptr<RenderComponent> RenderComponent;
        std::shared_ptr<RigidbodyComponent> RigidbodyComponent;
        std::shared_ptr<ColliderComponent> ColliderComponent;
        std::shared_ptr<AnimatorComponent> AnimatorComponent;
        //std::shared_ptr<PawnMovementComponent> MovementComponent;

        //Updates the character
        virtual void OnUpdate(float deltaTime) {};


    protected:
        //PawnMovementComponent* MovementComponent;  // Movement component for handling input-driven movement
        Character(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));
        virtual ~Character();
    };
}

