#include <Beginning.h>
#ifdef _WIN32
#include <direct.h>   // for _getcwd
#define GetCurrentDir _getcwd


#else
#include <unistd.h>   // for getcwd
#define GetCurrentDir getcwd
#endif

using namespace Beginning;

class MediumWallLeft : public Beginning::Object {

public:

	int counter = 0;

	MediumWallLeft()
	{
		this->SetPosition(glm::vec3(100.0f, 90.0f,0.0f));

		auto renderComponent = RenderComponent::Create();
		this->AddComponent(renderComponent);

		Beginning::Tilemap tilemap(4, 8, 17, 20);
		
		//When setting tiles for the tilemap, the user doesnt need to set the width and scale, because the tilemap settings replace those values
		Beginning::Sprite Sprite("Assets/Xenon2000/Blocks.bmp", glm::vec2(16, 64), glm::vec2(14, 49), true);

		Beginning::Sprite Sprite1("Assets/Xenon2000/Blocks.bmp", glm::vec2(16, 64), glm::vec2(9, 42), true);
		Beginning::Sprite Sprite2("Assets/Xenon2000/Blocks.bmp", glm::vec2(16, 64), glm::vec2(10, 42), true);
		Beginning::Sprite Sprite3("Assets/Xenon2000/Blocks.bmp", glm::vec2(16, 64), glm::vec2(11, 42), true);

		Beginning::Sprite Sprite4("Assets/Xenon2000/Blocks.bmp", glm::vec2(16, 64), glm::vec2(9, 43), true);
		Beginning::Sprite Sprite5("Assets/Xenon2000/Blocks.bmp", glm::vec2(16, 64), glm::vec2(10, 43), true);
		Beginning::Sprite Sprite6("Assets/Xenon2000/Blocks.bmp", glm::vec2(16, 64), glm::vec2(11, 43), true);

		Beginning::Sprite Sprite7("Assets/Xenon2000/Blocks.bmp", glm::vec2(16, 64), glm::vec2(6, 44), true);
		Beginning::Sprite Sprite8("Assets/Xenon2000/Blocks.bmp", glm::vec2(16, 64), glm::vec2(7, 44), true);
		Beginning::Sprite Sprite9("Assets/Xenon2000/Blocks.bmp", glm::vec2(16, 64), glm::vec2(10, 44), true);
		Beginning::Sprite Sprite10("Assets/Xenon2000/Blocks.bmp", glm::vec2(16, 64), glm::vec2(11, 44), true);

		Beginning::Sprite Sprite11("Assets/Xenon2000/Blocks.bmp", glm::vec2(16, 64), glm::vec2(6, 45), true);
		Beginning::Sprite Sprite12("Assets/Xenon2000/Blocks.bmp", glm::vec2(16, 64), glm::vec2(7, 45), true);
		Beginning::Sprite Sprite13("Assets/Xenon2000/Blocks.bmp", glm::vec2(16, 64), glm::vec2(10, 45), true);
		Beginning::Sprite Sprite14("Assets/Xenon2000/Blocks.bmp", glm::vec2(16, 64), glm::vec2(11, 45), true);

		Beginning::Sprite Sprite15("Assets/Xenon2000/Blocks.bmp", glm::vec2(16, 64), glm::vec2(5, 46), true);
		Beginning::Sprite Sprite16("Assets/Xenon2000/Blocks.bmp", glm::vec2(16, 64), glm::vec2(10, 46), true);
		Beginning::Sprite Sprite17("Assets/Xenon2000/Blocks.bmp", glm::vec2(16, 64), glm::vec2(11, 46), true);

		Beginning::Sprite Sprite18("Assets/Xenon2000/Blocks.bmp", glm::vec2(16, 64), glm::vec2(7, 47), true);
		Beginning::Sprite Sprite19("Assets/Xenon2000/Blocks.bmp", glm::vec2(16, 64), glm::vec2(10, 47), true);

		Beginning::Sprite Sprite20("Assets/Xenon2000/Blocks.bmp", glm::vec2(16, 64), glm::vec2(11, 47), true);

		Beginning::Sprite Invisible("", glm::vec2(16, 64), glm::vec2(11, 47), true);

		tilemap.SetTile(0, 0, std::make_shared<Beginning::Sprite>(Invisible));
		tilemap.SetTile(0, 1, std::make_shared<Beginning::Sprite>(Sprite18));
		tilemap.SetTile(0, 2, std::make_shared<Beginning::Sprite>(Sprite15));
		tilemap.SetTile(0, 3, std::make_shared<Beginning::Sprite>(Sprite11));
		tilemap.SetTile(0, 4, std::make_shared<Beginning::Sprite>(Sprite7));
		tilemap.SetTile(0, 5, std::make_shared<Beginning::Sprite>(Sprite4));
		tilemap.SetTile(0, 6, std::make_shared<Beginning::Sprite>(Sprite1));
		tilemap.SetTile(0, 7, std::make_shared<Beginning::Sprite>(Sprite));

		tilemap.SetTile(1, 0, std::make_shared<Beginning::Sprite>(Invisible));
		tilemap.SetTile(1, 1, std::make_shared<Beginning::Sprite>(Sprite19));
		tilemap.SetTile(1, 2, std::make_shared<Beginning::Sprite>(Sprite16));
		tilemap.SetTile(1, 3, std::make_shared<Beginning::Sprite>(Sprite12));
		tilemap.SetTile(1, 4, std::make_shared<Beginning::Sprite>(Sprite8));
		tilemap.SetTile(1, 5, std::make_shared<Beginning::Sprite>(Sprite5));
		tilemap.SetTile(1, 6, std::make_shared<Beginning::Sprite>(Sprite2));
		tilemap.SetTile(1, 7, std::make_shared<Beginning::Sprite>(Invisible));
		
		
		tilemap.SetTile(2, 0, std::make_shared<Beginning::Sprite>(Invisible));
		tilemap.SetTile(2, 1, std::make_shared<Beginning::Sprite>(Invisible));
		tilemap.SetTile(2, 2, std::make_shared<Beginning::Sprite>(Sprite17));
		tilemap.SetTile(2, 3, std::make_shared<Beginning::Sprite>(Sprite13));
		tilemap.SetTile(2, 4, std::make_shared<Beginning::Sprite>(Sprite9));
		tilemap.SetTile(2, 5, std::make_shared<Beginning::Sprite>(Sprite6));
		tilemap.SetTile(2, 6, std::make_shared<Beginning::Sprite>(Sprite3));
		tilemap.SetTile(2, 7, std::make_shared<Beginning::Sprite>(Invisible));


		tilemap.SetTile(3, 0, std::make_shared<Beginning::Sprite>(Invisible));
		tilemap.SetTile(3, 1, std::make_shared<Beginning::Sprite>(Invisible));
		tilemap.SetTile(3, 2, std::make_shared<Beginning::Sprite>(Invisible));
		tilemap.SetTile(3, 3, std::make_shared<Beginning::Sprite>(Sprite14));
		tilemap.SetTile(3, 4, std::make_shared<Beginning::Sprite>(Sprite10));
		tilemap.SetTile(3, 5, std::make_shared<Beginning::Sprite>(Invisible));
		tilemap.SetTile(3, 6, std::make_shared<Beginning::Sprite>(Invisible));
		tilemap.SetTile(3, 7, std::make_shared<Beginning::Sprite>(Invisible));


		renderComponent->SetTilemap(std::make_shared<Beginning::Tilemap>(tilemap), glm::vec3(0.0f,0.0f,0.0f), -90.f);
		
	}

	~MediumWallLeft()
	{
	}

	// Overridden Update Method
	void OnUpdate(float deltaTime) override
	{
		this->SetPosition(glm::vec3(this->GetPosition().x - 0.1f, this->GetPosition().y, this->GetPosition().z));
		counter++;
		if (counter == 5000)
		{
			Application::GetObjectManager().DestroyObject(this);
		}
	}

};