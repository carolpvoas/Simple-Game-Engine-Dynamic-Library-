#include <Beginning.h>
#include "Rock.h"

#ifdef _WIN32
#include <direct.h>   // for _getcwd
#define GetCurrentDir _getcwd


#else
#include <unistd.h>   // for getcwd
#define GetCurrentDir getcwd
#endif

class Loner : public Beginning::Character {

public:


	int delayTime = 300;
	Player* player;


	Loner()
	{
		this->SetPosition(glm::vec3(50.0f, 0.0f, 0.0f));

		Beginning::Sprite Sprite("Assets/Xenon2000/LonerA.bmp", glm::vec2(4, 4), glm::vec2(1, 1), 20, 20);

		RenderComponent->SetSprite(std::make_shared<Beginning::Sprite>(Sprite), glm::vec3(0.0f,0.0f,0.0f), 1.0f, 90.f);

		ColliderComponent->SetCircleRadius(0.1f);
		ColliderComponent->OnCollisionAddDynamic(this, &Loner::OnCollision);

		RigidbodyComponent->SetBodyType(b2_staticBody);
		RigidbodyComponent->ActiveRigidbody(true);
		RigidbodyComponent->SetGravityScale(0.0f);
		RigidbodyComponent->SetBodyDesinty(100000.0f);

		this->Tag = "enemy";
		//GetComponent<Beginning::RenderComponent>()->RenderCollider();
	}

	~Loner()
	{
		//delete this;
	}

	// Overridden Update Method
	void OnUpdate(float deltaTime) override
	{
		delayTime++;
		if (delayTime >= 100)
		{
			if (player != nullptr)
			{
				delayTime = 0;
				Rock* rock = Application::GetObjectManager().CreateObject<Rock>();
				rock->StartPosition(this->GetPosition() + glm::vec3(0.0f, -10.f, 0.0f), player);
			}

		}
	}

	void OnCollision(Beginning::CollisionEvent& event)
	{
		Object* otherObject = static_cast<Object*>(event.collidee);
		Object* otherObject2 = static_cast<Object*>(event.collider);
		if (otherObject->Tag == "missile" || otherObject2->Tag == "missile")
		{
			Application::GetObjectManager().DestroyObject(this);
		}
	}

	void AddPlayerReference(Player* player)
	{
		this->player = player;
	}


};