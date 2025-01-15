#include "Character.h"

namespace Beginning{

	Character::Character(glm::vec3 position)
	{
		RenderComponent = RenderComponent::Create();
		//collider = ColliderComponent::Create(ColliderType::Circle, glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.2f);
		ColliderComponent = ColliderComponent::Create(ColliderType::Circle, glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.2f);
		RigidbodyComponent = RigidbodyComponent::Create(glm::vec3(0.0f, 0.0f, 0.0f), b2_dynamicBody);
		AnimatorComponent = AnimatorComponent::Create();
		//MovementComponent = PawnMovementComponent::Create();
	
		this->AddComponent(RenderComponent);
		this->AddComponent(RigidbodyComponent);
		this->AddComponent(ColliderComponent);
		this->AddComponent(AnimatorComponent);
		//this->AddComponent(MovementComponent);
	}

	Character::~Character()
	{
	}

}
