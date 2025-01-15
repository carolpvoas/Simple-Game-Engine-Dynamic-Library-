#pragma once
#include <Beginning.h>
#include "Explosion.h"
#ifdef _WIN32
#include <direct.h>   // for _getcwd
#define GetCurrentDir _getcwd


#else
#include <unistd.h>   // for getcwd
#define GetCurrentDir getcwd
#endif

using namespace Beginning;

class Missile : public Beginning::Character {

public:

	Explosion* explosion;

	//Obstacles

	Missile()
	{
		this->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

		this->RemoveComponent<Beginning::AnimatorComponent>();

		//When setting tiles for the tilemap, the user doesnt need to set the width and scale, because the tilemap settings replace those values
		Beginning::Sprite Sprite("Assets/Xenon2000/hmissile.bmp", glm::vec2(4, 2), glm::vec2(1, 2), 20, 20);

		RenderComponent->SetSprite(std::make_shared<Beginning::Sprite>(Sprite), glm::vec3(0.0f,0.0f,0.0f), 1.0f, -90.f);
		ColliderComponent->SetCircleRadius(0.02f);
		ColliderComponent->OnCollisionAddDynamic(this, &Missile::OnCollision);

		this->Tag = "missile";

	}

	~Missile()
	{

	}

	void StartPosition(glm::vec3 initalPositionObject)
	{
		this->SetPosition(initalPositionObject);
		RigidbodyComponent->ActiveRigidbody(true);
		RigidbodyComponent->SetGravityScale(0.0f);
	}

	// Overridden Update Method
	void OnUpdate(float deltaTime) override
	{
		RigidbodyComponent->SetLinearVelocity(glm::vec3(100.0f, 0.f, 0.0f));

	}

	void OnCollision(Beginning::CollisionEvent& event)
	{
		Object* otherObject = static_cast<Object*>(event.collider);
		Object* otherObject2 = static_cast<Object*>(event.collidee);

		if (otherObject->Tag == "enemy" || otherObject2->Tag == "enemy")
		{
			explosion = Application::GetObjectManager().CreateObject<Explosion>();
			explosion->SetPosition(this->GetPosition() + glm::vec3(0.0f, 0.0f, 10.f));
			Application::GetObjectManager().DestroyObject(this);
		}
		else if (otherObject->Tag != "player" || otherObject2->Tag != "player")
		{
			Application::GetObjectManager().DestroyObject(this);
		}
		
	}

};