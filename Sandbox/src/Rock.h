#pragma once
#include <Beginning.h>
#include "Player.h"
#ifdef _WIN32
#include <direct.h>   // for _getcwd
#define GetCurrentDir _getcwd


#else
#include <unistd.h>   // for getcwd
#define GetCurrentDir getcwd
#endif

using namespace Beginning;

class Rock : public Beginning::Character {

public:

	std::shared_ptr<PawnMovementComponent> movementComponent;

	Rock()
	{
		this->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

		Beginning::Sprite Sprite("Assets/Xenon2000/EnWeap6.bmp", glm::vec2(8, 1), glm::vec2(0, 0), 10, 10);
		RenderComponent->SetSprite(std::make_shared<Beginning::Sprite>(Sprite));

		AnimatorComponent->AddAnimation(Beginning::Animation("Idle", glm::ivec2(0, 1), glm::ivec2(7, 1), true, 0.1f));
		AnimatorComponent->SetAnimation("Idle");

		movementComponent = PawnMovementComponent::Create();
		this->AddComponent(movementComponent);
		movementComponent->SetMovementVelocity(40.f);

		ColliderComponent->OnCollisionAddDynamic(this, &Rock::OnCollision);
		ColliderComponent->SetCircleRadius(0.02f);

		this->Tag = "enemy";
	}

	~Rock()
	{
	}

	// Overridden Update Method
	void OnUpdate(float deltaTime) override
	{
		if (movementComponent->CheckIfMoving() == false)
		{
			Application::GetObjectManager().DestroyObject(this);
		}
	}

	void StartPosition(glm::vec3 initalPositionObject, Player* playerReference)
	{
		this->SetPosition(initalPositionObject);
		movementComponent->MoveTo(nullptr, (playerReference->GetPosition() * glm::vec3(6.f, 6.f, 0.0f)));
	}

	void OnCollision(Beginning::CollisionEvent& event)
	{
		Object* otherObject = static_cast<Object*>(event.collider);
		Object* otherObject2 = static_cast<Object*>(event.collidee);
		
		if (otherObject->Tag != "enemy" && otherObject->Tag != "enemy")
		{
			Application::GetObjectManager().DestroyObject(this);
		}

	}

};