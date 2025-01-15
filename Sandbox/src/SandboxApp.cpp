#include <Beginning.h>
#include "Sandbox.h"


#ifdef _WIN32
#include <direct.h>   // for _getcwd
#define GetCurrentDir _getcwd


#else
#include <unistd.h>   // for getcwd
#define GetCurrentDir getcwd
#endif

using namespace Beginning;


	Sandbox::Sandbox()
	{
		playerUI = new PlayerUI();
		//-----------------BACKGROUND----------------------
		//Creates the object
		background = GetObjectManager().CreateObject<Object>();

		background->SetPosition(glm::vec3(0.0f, 0.0f, -10.0f));

		//Creates a render component
		auto renderComponent = RenderComponent::Create();

		//Adds the render component to the object
		background->AddComponent(renderComponent);

		//Creates a sprite
		Beginning::Sprite backgroundSprite("Assets/Xenon2000/galaxy2.bmp", glm::vec2(1, 1), glm::vec2(1, 1), 200, 200, glm::vec3(200.f, 200.f, 1.f));

		//Sets the sprite to the render component
		renderComponent->SetSprite(std::make_shared<Beginning::Sprite>(backgroundSprite), glm::vec3(0.0f, 0.0f, -5.0f), 1.0f, 90.f);

		//---------------------------------------------------
		
		MediumWall = GetObjectManager().CreateObject<MediumWallLeft>();
		Enemy = GetObjectManager().CreateObject<Loner>();
		myPlayer = GetObjectManager().CreateObject<Player>();
		companion = GetObjectManager().CreateObject<Companion>();
		Enemy->AddPlayerReference(myPlayer);


		GetInput().BindPlayer(myPlayer);

		shieldPowerUp = GetObjectManager().CreateObject<PowerUp>();
		shieldPowerUp->SetPowerUp(PowerUp::Type::Shield);

	}

	Sandbox::~Sandbox()
	{
		delete playerUI;
	}


	void Sandbox::OnUpdate()
	{ 
		if (myPlayer != nullptr)
		{
			if (myPlayer->isPendingKill)
			{
				myPlayer = nullptr;
			}
			else if (myPlayer->fullLife == true)
			{
				playerLifes = 3;
				myPlayer->fullLife = false;
				playerUI->SetFullHealth();
			}
		}
		

		if (myPlayer == nullptr)
		{
			UpdatePlayerLife();

			if (companion != nullptr)
			{
				if (companion->isAttached)
				{
					GetObjectManager().DestroyObject(companion);
					companion = nullptr;
				}
			}
			
		}
		
		if (companion != nullptr)
		{
			if (companion->isAttached)
			{
				myPlayer->hasCompanion = true;
				companion->SetPosition(myPlayer->GetPosition() + glm::vec3(0.0f, 30.f, 0.0f));
			}
		}
		
	}

	void Sandbox::UpdatePlayerLife()
	{
		delayDead++;
		if (alreadySetPlayerLifes == false)
		{
			playerLifes--;
			playerUI->SetPlayerLifes();
			alreadySetPlayerLifes = true;
			if (playerLifes == 0)
			{
				gameEnded = true;
				text = GetUIManager().CreateUIObject<UIText>();
				text->SetText("Dead", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), TTF_OpenFont("Assets/Fonts/ARIAL.ttf", 30), 30);
			}
		}
		if (delayDead >= 100 && gameEnded == false)
		{
			myPlayer = GetObjectManager().CreateObject<Player>();
			GetInput().BindPlayer(myPlayer);
			alreadySetPlayerLifes = false;
			delayDead = 0;
		}
	}

	void Sandbox::AddCompanion()
	{
		companion = GetObjectManager().CreateObject<Companion>();
	}




