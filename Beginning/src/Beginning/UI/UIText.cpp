#include "UIText.h"
#include "../Renderer/Renderer.h"

namespace Beginning{
	
	void UIText::SetText(std::string text, glm::vec4 color, TTF_Font* font, int size, glm::vec3 position)
	{
		m_Renderer->AddTextRender(this, text, color, font, size, position);
	}

	void Beginning::UIText::Update(float deltaTime)
	{
	}

	UIText::UIText()
	{
	}

	UIText::~UIText()
	{
	}
}
