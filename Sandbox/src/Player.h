#pragma once
#include <Beginning.h>
#include "Missile.h"
#include "Companion.h"
#ifdef _WIN32
#include <direct.h>   // for _getcwd
#define GetCurrentDir _getcwd


#else
#include <unistd.h>   // for getcwd
#define GetCurrentDir getcwd
#endif

using namespace Beginning;

class Missile;

class Player : public Beginning::Character {
public:
	
	enum LateralGas
	{
		DownLeft,
		DownRight,
		UpLeft,
		UpRight
	};

	std::shared_ptr<Beginning::RenderComponent> renderComponentBurner;
	std::shared_ptr<Beginning::RenderComponent> renderComponentBurnerLateral;

	glm::vec2 velocity = glm::vec2(0.0f, 0.0f);


	float height = 10.f;
	float offsetY = 0.0f;
	bool gasNotReduced = true;
	bool enoughGas = true;
	float opacity = 0.0f;

	bool stillExists = true;

	int playerLifes = 3;

	int delayTime;

	Missile* missile;
	Missile* companionMissile;

	bool hasCompanion = false;

	bool fullLife = false;


	Player()
	{
		this->SetPosition(glm::vec3(0.0f, 0.0f, 100.0f));

		Beginning::Sprite Sprite("Assets/Xenon2000/Ship1.bmp", glm::vec2(7, 1), glm::vec2(0, 0), 30, 30);
		RenderComponent->SetSprite(std::make_shared<Beginning::Sprite>(Sprite), glm::vec3(0.0f, 0.0f,0.0f), 1.0f, -90.0f);

		RigidbodyComponent->SetBodyType(b2_dynamicBody);
		RigidbodyComponent->SetGravityScale(0.0f);
		RigidbodyComponent->ActiveRigidbody(true);

		this->AnimatorComponent->AddAnimation(Beginning::Animation("Idle", glm::ivec2(3, 0), glm::ivec2(3, 0), true, 0.1f));
		this->AnimatorComponent->AddAnimation(Beginning::Animation("MovementRight", glm::ivec2(4, 0), glm::ivec2(6, 0), false, 0.1f));
		this->AnimatorComponent->AddAnimation(Beginning::Animation("MovementLeft", glm::ivec2(3, 0), glm::ivec2(1, 0), false, 0.1f));

		this->AnimatorComponent->SetAnimation("Idle");

		ColliderComponent->SetCircleRadius(0.1);
		ColliderComponent->OnCollisionAddDynamic(this, &Player::OnCollision);
		//	GetComponent<Beginning::RenderComponent>()->RenderCollider();

		renderComponentBurner = RenderComponent::Create();
		this->AddComponent(renderComponentBurner);
		Beginning::Sprite Burner("Assets/Xenon2000/Burner1.bmp", glm::vec2(1, 1), glm::vec2(0, 0), 10, 10);
		renderComponentBurner->SetSprite(std::make_shared<Beginning::Sprite>(Burner), glm::vec3(-20.0f, 0.f,0.0f), 1.0f, -90.0f);

		renderComponentBurnerLateral = RenderComponent::Create();
		this->AddComponent(renderComponentBurnerLateral);

		this->Tag = "player";


	}

	~Player()
	{
	}

	void OnUpdate(float deltatime) override
	{
		velocity.x = 0.0f;
		velocity.y = 0.0f;

		const float speed = 100.0f;
			if (!isPendingKill)
			{
				if (Application::Get().GetInput().KeyPressed(SDL_SCANCODE_A))
				{
					velocity.x -= speed; // Move left

					GiveLateralGas(LateralGas::DownRight, 0.01f);
					GiveLateralGas(LateralGas::UpRight, 0.01f);
					GiveLateralGas(LateralGas::DownLeft, 0.f);
					GiveLateralGas(LateralGas::UpLeft, 0.f);
					
					ReduceGas();
				}
				if (Application::Get().GetInput().KeyPressed(SDL_SCANCODE_D))
				{
					velocity.x += speed; // Move right

					GiveLateralGas(LateralGas::DownLeft, 0.01f);
					GiveLateralGas(LateralGas::UpLeft, 0.01f);
					GiveLateralGas(LateralGas::DownRight, 0.f);
					GiveLateralGas(LateralGas::UpRight, 0.f);
					GiveGas();
					
				}
				if (Application::Get().GetInput().KeyPressed(SDL_SCANCODE_W))
				{
					velocity.y += speed; // Move up
					
					
					GiveLateralGas(LateralGas::DownLeft, 0.01f);
					GiveLateralGas(LateralGas::DownRight, 0.01f);
					GiveLateralGas(LateralGas::UpLeft, 0.f);
					GiveLateralGas(LateralGas::UpRight, 0.f);
					if (AnimatorComponent->GetCurrentFrame() == AnimatorComponent->GetCurrentAnimation()->endFrame)
					{
						if (AnimatorComponent->GetCurrentFrame() == AnimatorComponent->GetCurrentAnimation()->endFrame && AnimatorComponent->GetCurrentAnimation()->name != "MovementLeft")
						{
							//Animação acabou
							AnimatorComponent->SetAnimation("MovementLeft");
						}
					}

				}
				if (Application::Get().GetInput().KeyPressed(SDL_SCANCODE_S))
				{
					velocity.y -= speed; // Move down
					
					GiveLateralGas(LateralGas::UpLeft, 0.01f);
					GiveLateralGas(LateralGas::UpRight, 0.01f);
					GiveLateralGas(LateralGas::DownRight, 0.f);
					GiveLateralGas(LateralGas::DownLeft, 0.f);
					if (AnimatorComponent->GetCurrentFrame() == AnimatorComponent->GetCurrentAnimation()->endFrame)
					{
						if (AnimatorComponent->GetCurrentFrame() == AnimatorComponent->GetCurrentAnimation()->endFrame && AnimatorComponent->GetCurrentAnimation()->name != "MovementRight")
						{
							//Animação acabou
							AnimatorComponent->SetAnimation("MovementRight");
						}
					}

					
				}

				if (velocity.x == 0 && velocity.y == 0)
				{
					//Animação acabou
					AnimatorComponent->SetAnimation("Idle");
					GiveGas();
					opacity = 0.0f;
					GiveLateralGas(LateralGas::DownLeft, 0.0f);
					GiveLateralGas(LateralGas::DownRight, 0.0f);
					GiveLateralGas(LateralGas::UpLeft, 0.0f);
					GiveLateralGas(LateralGas::UpRight, 0.0f);
				}

				if (glm::length(velocity) > 0) {
					velocity = glm::normalize(velocity) * speed;
				}


				RigidbodyComponent->SetLinearVelocity(velocity);

				delayTime++;

				if (Application::Get().GetInput().KeyPressed(SDL_SCANCODE_SPACE))
				{

					if (delayTime >= 10)
					{
						//TO-DO:: Not initialized yet the missile
						missile = Application::Get().GetObjectManager().CreateObject<Missile>();
 						missile->StartPosition(GetPosition() + glm::vec3(30.f, 0.f, 0.f));
						    
						if (hasCompanion)
						{
							companionMissile = Application::Get().GetObjectManager().CreateObject<Missile>();
							companionMissile->StartPosition(this->GetPosition() + glm::vec3(20.f, 30.f, 0.f));
						}
						
						delayTime = 0;
					}
				}

			}
	}

	void ReduceGas()
	{
		if (gasNotReduced)
		{
			height = height - 0.1f;
			offsetY = offsetY + 0.05f;
			Beginning::Sprite Burner1("Assets/Xenon2000/Burner1.bmp", glm::vec2(1, 1), glm::vec2(0, 0), height, 10);
			renderComponentBurner->SetSprite(std::make_shared<Beginning::Sprite>(Burner1), glm::vec3(-20.f + offsetY, 0.0f, 0.0f), 1.0f, -90.0f);
			enoughGas = false;
			
			if (height < 2.f)
			{
				gasNotReduced = false;
			}
		}
	}

	void GiveGas()
	{
		if (enoughGas == false)
		{
			height = height + 0.5f;
			offsetY = offsetY - 0.25f;
			Beginning::Sprite Burner1("Assets/Xenon2000/Burner1.bmp", glm::vec2(1, 1), glm::vec2(0, 0), height, 10);
			renderComponentBurner->SetSprite(std::make_shared<Beginning::Sprite>(Burner1), glm::vec3(-20.f + offsetY, 0.0f, 0.0f), 1.0f, -90.0f);
			if (height > 9.9f)
			{
				enoughGas = true;
				gasNotReduced = true;
			}
		}
	}

	void GiveLateralGas(Player::LateralGas lateralGas, float power)
	{
			switch (lateralGas)
			{
			case Player::LateralGas::DownLeft:
			{
				opacity = opacity + power;
				Beginning::Sprite DownRight("Assets/Xenon2000/Burner5.bmp", glm::vec2(2, 2), glm::vec2(1, 1), 6, 10);
				if (power != 0.0f)
				{
					renderComponentBurnerLateral->SetSprite(std::make_shared<Beginning::Sprite>(DownRight), glm::vec3(-20.0f, -20.f, -2.0f), opacity, -90.0f);
				}
				else
				{
					renderComponentBurnerLateral->SetSprite(std::make_shared<Beginning::Sprite>(DownRight), glm::vec3(-20.0f, -20.f, -2.0f), 0.0f, -90.0f);
				}
				renderComponentBurnerLateral->UpdateRenderPosition();
				break;
				
			}
			case Player::LateralGas::DownRight:
			{
		        opacity = opacity + power;
				Beginning::Sprite UpRight("Assets/Xenon2000/Burner4.bmp", glm::vec2(2, 2), glm::vec2(1, 1), 6, 10);
				if (power != 0.0f)
				{

					renderComponentBurnerLateral->SetSprite(std::make_shared<Beginning::Sprite>(UpRight), glm::vec3(5.f, -10.f, -1.0f), opacity, -90.0f);

				}
				else
				{
					renderComponentBurnerLateral->SetSprite(std::make_shared<Beginning::Sprite>(UpRight), glm::vec3(5.f, -10.f, -1.0f), 0.0f, -90.0f);
				}
				renderComponentBurnerLateral->UpdateRenderPosition();
				break;
			}

			case Player::LateralGas::UpLeft:
			{
				
				//downlef
				opacity = opacity + power;
				Beginning::Sprite DownLeft("Assets/Xenon2000/Burner6.bmp", glm::vec2(2, 2), glm::vec2(1, 1), 6, 10);
				if (power != 0.0f)
				{
					renderComponentBurnerLateral->SetSprite(std::make_shared<Beginning::Sprite>(DownLeft), glm::vec3(-20.0f, 10.f, -2.0f), opacity, -90.0f);
				}
				else
				{
					renderComponentBurnerLateral->SetSprite(std::make_shared<Beginning::Sprite>(DownLeft), glm::vec3(-20.0f, 10.f, -2.0f), 0.0f, -90.0f);
				}
				renderComponentBurnerLateral->UpdateRenderPosition();
				break;
				
			}
			case Player::LateralGas::UpRight:
			{
				opacity = opacity + power;
				Beginning::Sprite UpLeft("Assets/Xenon2000/Burner3.bmp", glm::vec2(2, 2), glm::vec2(1, 1), 6, 10);
				if (power != 0.0f)
				{
					renderComponentBurnerLateral->SetSprite(std::make_shared<Beginning::Sprite>(UpLeft), glm::vec3(5.f, 10.f, -1.0f), opacity, -90.0f);

				}
				else
				{
					renderComponentBurnerLateral->SetSprite(std::make_shared<Beginning::Sprite>(UpLeft), glm::vec3(5.f, 10.f, -1.0f), 0.0f, -90.0f);
				}
				renderComponentBurnerLateral->UpdateRenderPosition();
				break;
				
			}
		}
		
	}
	
	void OnCollision(Beginning::CollisionEvent& event)
	{
		Object* otherObject = static_cast<Object*>(event.collider);
		Object* otherObject2 = static_cast<Object*>(event.collidee);
		if (otherObject->Tag == "enemy" || otherObject2->Tag == "enemy")
		{
			Application::GetObjectManager().DestroyObject(this);
		}
		else if (otherObject->Tag == "companion" || otherObject2->Tag == "companion")
		{
		}
		else if (otherObject->Tag == "shield" || otherObject2->Tag == "shield")
		{
			fullLife = true;
		}
	}

	
};
