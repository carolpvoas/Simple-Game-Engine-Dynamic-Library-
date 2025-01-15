#pragma once
#include "Component.h"
#include "../Objects/Object.h"
#include "glm.hpp"

namespace Beginning {

    class Pawn; // Forward declaration

    class PawnMovementComponent : public Component {
    public:

        // Factory method to create new PawnMovementComponent instances
        static std::shared_ptr<PawnMovementComponent> Create() {
            return std::shared_ptr<PawnMovementComponent>(new PawnMovementComponent());
        }

        void Update(float deltaTime) override;
        void Start() override;
        void Shutdown() override;

		void SetMovementVelocity(float velocity) { m_MovementVelocity = velocity; }

        void MoveTo(Object* object = nullptr, glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f));

        bool CheckIfMoving() { return m_IsMoving; }

    private:

        PawnMovementComponent();
        
        Pawn* m_OwnerPawn;
        float m_MovementVelocity = 1.0f;
        glm::vec3 m_TargetPosition; // New member to store the target position
        bool m_IsMoving = false;    // New member to track movement status

    };

}
