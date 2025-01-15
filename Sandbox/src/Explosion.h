#include <Beginning.h>
#ifdef _WIN32
#include <direct.h>   // for _getcwd
#define GetCurrentDir _getcwd


#else
#include <unistd.h>   // for getcwd
#define GetCurrentDir getcwd
#endif

using namespace Beginning;

class Explosion : public Beginning::Object {

public:
	int counter = 0;

	Explosion()
	{
		auto renderComponent = RenderComponent::Create();
		this->AddComponent(renderComponent);

		auto animatorComponent = AnimatorComponent::Create();
		this->AddComponent(animatorComponent);

		Beginning::Sprite Sprite("Assets/Xenon2000/explode64.bmp", glm::vec2(5, 2), glm::vec2(1, 0), 30, 30);
		renderComponent->SetSprite(std::make_shared<Beginning::Sprite>(Sprite));

		animatorComponent->AddAnimation(Beginning::Animation("ExplosionAnim", glm::ivec2(0, 1), glm::ivec2(4, 2), false, 0.05f));
		animatorComponent->SetAnimation("ExplosionAnim");
	}

	~Explosion()
	{
	}

	// Overridden Update Method
	void OnUpdate(float deltaTime) override
	{
		counter++;
		if (counter == 30)
		{
			Application::GetObjectManager().DestroyObject(this);
		}
	}

};