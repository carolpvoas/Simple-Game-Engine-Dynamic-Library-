#include "UIObject.h"
#include "../Renderer/Renderer.h"


namespace Beginning
{
    void UIObject::Update(float deltaTime)
    {
		if (IsVisible != currentVisibility)
        {
			currentVisibility = IsVisible;
            UpdateRender(currentPosition, m_Size, currentVisibility);
		}
    }
    void UIObject::SetAlignment(UIAlignment newAlignment, glm::vec3 offset)
	{
        // Get screen dimensions
        int screenWidth = (600 / 3);
        int screenHeight = (600 / 3);

        glm::vec2 position; 

        // Calculate position based on alignment
        switch (newAlignment) {
        case UIAlignment::TopLeft:
            position = glm::vec2(0.0f, screenHeight - m_Size.y);  // Top-left corner
            break;
        case UIAlignment::TopRight:
            position = glm::vec2(screenWidth - m_Size.x, screenHeight - m_Size.y);  // Top-right corner
            break;
        case UIAlignment::BottomLeft:
            position = glm::vec2(-(screenWidth / 2), -(screenHeight/2));  // Bottom-left corner
            break;
        case UIAlignment::BottomRight:
            position = glm::vec2(screenWidth - m_Size.x, 0.0f);  // Bottom-right corner
            break;
        case UIAlignment::Center:
            position = glm::vec2((screenWidth - m_Size.x) / 2, (screenHeight - m_Size.y) / 2);  // Centered
            break;
        }

        currentPosition = glm::vec3(glm::vec3(position, 0.0f) + offset);
        UpdateRender(currentPosition, m_Size, currentVisibility);
	}

	UIObject::UIObject() : m_Renderer(&Renderer::GetInstance())
	{
	}

	UIObject::~UIObject()
	{
	}
	
	void UIObject::CleanUIObject()
	{
	}
}

