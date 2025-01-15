#include <Beginning.h>
#ifdef _WIN32
#include <direct.h>   // for _getcwd
#define GetCurrentDir _getcwd


#else
#include <unistd.h>   // for getcwd
#define GetCurrentDir getcwd
#endif

using namespace Beginning;

class PlayerUI : public Beginning::UIObject {

public:

	UIImage* Life1;
	UIImage* Life2;
	UIImage* Life3;

	int lifes = 3;


	PlayerUI()
	{
		Life1 = Application::GetUIManager().CreateUIObject<UIImage>();

		Beginning::Sprite imageSprite("Assets/Xenon2000/Ship1.bmp", glm::vec2(7, 1), glm::vec2(4, 1), 20, 20);
		Life1->SetImage(std::make_shared<Beginning::Sprite>(imageSprite));

		Life1->SetAlignment(UIAlignment::BottomLeft, glm::vec3(20.f, 20.f, 0.0f));

		Life2 = Application::GetUIManager().CreateUIObject<UIImage>();
		Life2->SetImage(std::make_shared<Beginning::Sprite>(imageSprite));
		Life2->SetAlignment(UIAlignment::BottomLeft, glm::vec3(42.f, 20.f, 0.0f));

		Life3 = Application::GetUIManager().CreateUIObject<UIImage>();
		Life3->SetImage(std::make_shared<Beginning::Sprite>(imageSprite));
		Life3->SetAlignment(UIAlignment::BottomLeft, glm::vec3(64.f, 20.f, 0.0f));
	}

	~PlayerUI()
	{
	}

	void SetPlayerLifes()
	{
		lifes--;
		if (lifes == 2)
		{
			Life3->IsVisible = false;
		}
		else if (lifes == 1)
		{
			Life2->IsVisible = false;
		}
		else if (lifes == 0)
		{
			Life1->IsVisible = false;
		}
		
	}

	void SetFullHealth()
	{
		lifes = 3;
		Life1->IsVisible = true;
		Life2->IsVisible = true;
		Life3->IsVisible = true;
	}



};