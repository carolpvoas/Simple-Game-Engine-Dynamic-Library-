#include <Beginning.h>

#include "Beginning/EntryPoint.h"
#include "Beginning/Application.h"

#include "MediumWallLeft.h"
#include "Loner.h"
#include "Player.h"
#include "Companion.h"

#include "PlayerUI.h"

#include "PowerUp.h"

#include "Rock.h"

#ifdef _WIN32
#include <direct.h>   // for _getcwd
#define GetCurrentDir _getcwd


#else
#include <unistd.h>   // for getcwd
#define GetCurrentDir getcwd
#endif

using namespace Beginning;

class Player;


class Sandbox : public Beginning::Application
{

public:

		Object* background;
		MediumWallLeft* MediumWall;
		Player* myPlayer;
		Loner* Enemy;
		Companion* companion;
		PowerUp* shieldPowerUp;

		PlayerUI* playerUI;

		UIText* text;

		Rock* rock;

		bool canBeIdle = true;

		

		int delayTime = 0;
		int delayDead = 0;

		int playerLifes = 3;
		bool alreadySetPlayerLifes = false;

		bool gameEnded = false;

		std::shared_ptr<Beginning::AnimatorComponent> playerAnimator;
		std::shared_ptr<Beginning::RigidbodyComponent> playerRigidBody;

	Sandbox();
	~Sandbox();

	void OnUpdate() override;

	void UpdatePlayerLife();

	void AddCompanion();


};

Beginning::Application* Beginning::CreateApplication() {
	return new Sandbox();
}

