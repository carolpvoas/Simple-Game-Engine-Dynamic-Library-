#include "PawnMovementComponent.h"
#include "../Objects/Pawn.h"
#include "../Objects/Character.h"

namespace Beginning {

    PawnMovementComponent::PawnMovementComponent() : m_OwnerPawn(nullptr) {}

    void PawnMovementComponent::Start() {
        // Ensure that the owner is a Pawn
        m_OwnerPawn = dynamic_cast<Pawn*>(m_Owner);
        if (!m_OwnerPawn) {
            // Log error: Owner is not a pawn.
            m_OwnerPawn = dynamic_cast<Character*>(m_Owner);
        }

		if (!m_OwnerPawn) {
			//BG_ERROR("PawnMovementComponent Ãttached To An Object That Is Not A Pawn.");
			std::terminate();
		}
    }

    void PawnMovementComponent::Shutdown()
    {
    }

    void PawnMovementComponent::MoveTo(Object* object, glm::vec3 position)
    {
        if (!m_OwnerPawn) return;

        /*if (position == glm::vec3(0.0f, 0.0f, 0.0f) && object == nullptr)
         {
             //BG_ERROR("The MoveTo Function Needs A Target - PawnMovementComponent.h");
             std::terminate();
         }
         else if (position != glm::vec3(0.0f, 0.0f, 0.0f) && object != nullptr)
         {
            // BG_ERROR("The MoveTo Function Cannot Have Two Targets - PawnMovementComponent.h");
             std::terminate();
         }
         else if (position == glm::vec3(0.0f, 0.0f, 0.0f))
         {
             m_TargetPosition = object->GetPosition();
         }*/
        if (object == nullptr)
        {
            m_TargetPosition = position;
        }
        else
        {
            m_TargetPosition = object->GetPosition();
        }
            
            
        m_IsMoving = true;  // Set flag to start movement

        

    }

    void PawnMovementComponent::Update(float deltaTime) {
        if (!m_OwnerPawn || !m_IsMoving)
        {
            return; // Check if pawn should be moving
        }
		//TO-DO: CHECK IF I CAN USE THIS WITHOUT A RIGIDBODY. AND IF IT WAS, WHAT TYPE OF BODY SHOULD BE AND IF THE RIGIDBODY OVERRIDES THE POSITION
        //Do one version for rigidbody bodies, and one for objects that dont have one

        // Get the current position of the pawn
        glm::vec3 currentPosition = m_OwnerPawn->GetPosition();
        std::cout << "Current Position: (" << currentPosition.x << ", " << currentPosition.y << ", " << currentPosition.z << ")" << std::endl;

        // Calculate the vector towards the target position
        glm::vec3 direction = m_TargetPosition - currentPosition;
        float distanceToTarget = glm::length(direction);

        std::cout << "Direction: (" << direction.x << ", " << direction.y << ", " << direction.z << ")" << std::endl;
        std::cout << "Distance to Target: " << distanceToTarget << std::endl;


        // Normalize the direction to get the movement direction
        glm::vec3 normalizedDirection = glm::normalize(direction);

        // Calculate the distance to move in this frame
        float distanceToMove = m_MovementVelocity * deltaTime;

        // Check if the pawn is close enough to the target position
        if (distanceToTarget <= distanceToMove) {
            // Snap to the target position and stop moving
            m_OwnerPawn->SetPosition(m_TargetPosition);
            m_IsMoving = false;
            std::cout << "Pawn reached the target position." << std::endl;
        }
        else {
            // Move towards the target
            glm::vec3 newPosition = currentPosition + normalizedDirection * distanceToMove;
            m_OwnerPawn->SetPosition(newPosition);
            std::cout << "Moving pawn to: (" << newPosition.x << ", " << newPosition.y << ", " << newPosition.z << ")" << std::endl;
        }

        
    }

}
