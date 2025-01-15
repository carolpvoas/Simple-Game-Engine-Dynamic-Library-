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

class PowerUp : public Beginning::Character {
public:

	enum class Type
	{
		Shield
	};

	bool isSet = false;
	bool isPickedUp = false;

	int delayTime = 0;

	PowerUp()
	{
		

	}

	~PowerUp()
	{
	}

	void OnUpdate(float deltaTime) override
	{
		if (isSet)
		{
			RigidbodyComponent->SetLinearVelocity(glm::vec3(-5.0f, 0.0f, 0.0f));
		}
		
	}


	void OnCollision(Beginning::CollisionEvent& event)
	{
		Object* otherObject = static_cast<Object*>(event.collider);
		if (otherObject->Tag == "player")
		{
			isPickedUp = true;
			Application::GetObjectManager().DestroyObject(this);


			
		}

	}

	void SetPowerUp(const Type type)
	{
		this->SetPosition(glm::vec3(150.0f, -30.0f, 0.0f));

		switch (type)
		{
			case Type::Shield:
			{
				Beginning::Sprite Sprite("Assets/Xenon2000/PUShield.bmp", glm::vec2(4, 2), glm::vec2(1, 0), 20, 20);
				RenderComponent->SetSprite(std::make_shared<Beginning::Sprite>(Sprite), glm::vec3(0.0f,0.0f,0.0f), 1.0f, 90.f);

				this->AnimatorComponent->AddAnimation(Beginning::Animation("Idle", glm::ivec2(0, 1), glm::ivec2(3, 2), true, 0.1f));

				this->AnimatorComponent->SetAnimation("Idle");

				this->Tag = "shield";

			}
		}



		RigidbodyComponent->SetBodyType(b2_dynamicBody);
		RigidbodyComponent->SetGravityScale(0.0f);


		ColliderComponent->SetCircleRadius(0.1);
		ColliderComponent->OnCollisionAddDynamic(this, &PowerUp::OnCollision);
		
		RigidbodyComponent->ActiveRigidbody(true);

		//	GetComponent<Beginning::RenderComponent>()->RenderCollider();


		isSet = true;
	}


};
