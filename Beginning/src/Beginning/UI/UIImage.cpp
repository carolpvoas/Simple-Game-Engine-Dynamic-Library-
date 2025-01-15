#include "UIImage.h"
#include  "../Renderer/Sprite.h"


namespace Beginning{

	
	void UIImage::SetImage(std::shared_ptr<Sprite> newSprite, glm::vec3 spritePosition, float opacity, float rotation)
	{
		m_Size = glm::vec2(newSprite->GetWidth(), newSprite->GetHeight());
		m_Renderer->AddUIRender(this, newSprite->GetTextureString(), spritePosition, newSprite->GetWidth(), newSprite->GetHeight(), newSprite->GetScale(), newSprite->GetSpriteSheetDimensions(), newSprite->GetSpriteIndex(), opacity, rotation);
	}


	void UIImage::UpdateRender(glm::vec3 position, glm::vec2 size, bool isVisible)
	{
		m_Renderer->UpdateUIRender(this, position, size.x, size.y, isVisible);
	}

	UIImage::UIImage()
	{
	}

	UIImage::~UIImage()
	{
	}

}
