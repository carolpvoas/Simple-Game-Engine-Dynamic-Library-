#pragma once
#include <Beginning.h>
#ifdef _WIN32
#include <direct.h>   // for _getcwd
#define GetCurrentDir _getcwd


#else
#include <unistd.h>   // for getcwd
#define GetCurrentDir getcwd
#endif

using namespace Beginning;


class Companion : public Beginning::Character {
public:

	bool isAttached = false;
	
	Companion()
	{
		this->SetPosition(glm::vec3(100.0f, 20.0f, 0.0f));

		Beginning::Sprite Sprite("Assets/Xenon2000/clone.bmp", glm::vec2(4, 5), glm::vec2(1, 0), 20, 20);
		RenderComponent->SetSprite(std::make_shared<Beginning::Sprite>(Sprite), glm::vec3(0.0f,0.0f,0.0f),1.0f, 90.f);

		RigidbodyComponent->SetBodyType(b2_dynamicBody);
		RigidbodyComponent->SetGravityScale(0.0f);
		RigidbodyComponent->ActiveRigidbody(true);

		this->AnimatorComponent->AddAnimation(Beginning::Animation("Idle", glm::ivec2(0, 1), glm::ivec2(3, 3), true, 0.1f));

		this->AnimatorComponent->SetAnimation("Idle");

		ColliderComponent->SetCircleRadius(0.1);
		ColliderComponent->OnCollisionAddDynamic(this, &Companion::OnCollision);
		//	GetComponent<Beginning::RenderComponent>()->RenderCollider();

		this->Tag = "companion";

	}

	~Companion()
	{
	}

	void OnUpdate(float deltaTime) override
	{
		if (!isAttached)
		{
			RigidbodyComponent->SetLinearVelocity(glm::vec3(-5.f, 0.0f, 0.0f));
		}
	}
	

	void OnCollision(Beginning::CollisionEvent& event)
	{
		Object* otherObject = static_cast<Object*>(event.collider);
		Object* otherObject2 = static_cast<Object*>(event.collidee);

		if ((otherObject->Tag == "player" || otherObject2->Tag == "player"))
		{
			isAttached = true;

		}
		
	}


};
