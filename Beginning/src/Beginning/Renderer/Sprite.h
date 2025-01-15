#pragma once
#include <memory>
#include "Renderer.h"


namespace Beginning
{
    //CLASS DEFINITION: Hold the sprite data for the RenderComponent to use it

    class Sprite {
    public:
        Sprite(std::string texture, glm::vec2 spriteSheetDims, glm::vec2 spriteIndex, int width = 32, int height = 32, glm::vec3 scale = glm::vec3(0.f, 0.f, 0.f))
            : m_Texture(texture), m_Width(width), m_Height(height), m_Scale(scale), m_spriteSheetDims(spriteSheetDims), m_spriteIndex(spriteIndex), m_Tilemap(false) {
            
			//Reverts the couting, because the system counts from the bottom left corner, and we want to count from the top left corner
            m_spriteIndex.y = m_spriteSheetDims.y - m_spriteIndex.y;
            m_spriteIndex.y--;
            m_spriteIndex.x--;
            
        }

        //Gets the texture ID of the sprite
        std::string GetTextureID() const { return m_Texture; }

        //Gets the width of the sprite
        int GetWidth() const { return m_Width; }

        //Gets the height of the sprite
        int GetHeight() const { return m_Height; }

		std::string GetTextureString() const { return m_Texture; }

        //Gets the scaleof the sprite
        glm::vec3 GetScale() const { return m_Scale; }

        //Gets the spritesheet dimensions that provides the wanted sprite
        glm::vec2 GetSpriteSheetDimensions() const { return m_spriteSheetDims; }

        //Gets the spritesheet index that selectes the wanted sprite
        glm::vec2 GetSpriteIndex() const { return m_spriteIndex; }

    private:
        std::string m_Texture;
        int m_Width, m_Height;   
        glm::vec3 m_Scale;
        glm::vec2 m_spriteSheetDims;
        glm::vec2 m_spriteIndex;

        bool m_Tilemap = false;

    };

}