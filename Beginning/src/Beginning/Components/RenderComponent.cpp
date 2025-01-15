#include "RenderComponent.h"
#include "../Objects/Object.h"


namespace Beginning {

    int RenderComponent::GlobalID = 0;

	RenderComponent::RenderComponent() : m_Renderer(&Renderer::GetInstance()){
        ID = GlobalID++;
	}


    RenderComponent::~RenderComponent()
    {
		
    }

    void RenderComponent::Start()
    {
        //Gives to the owner access to the render component
        RenderComponent* renderComponent = dynamic_cast<RenderComponent*>(this);
        m_Owner->AddRenderComponent(renderComponent);
    }

    void RenderComponent::Update(float deltaTime) {
		
    }

    void RenderComponent::Shutdown()
    {
        std::cout << "RenderComponent destroyed" << std::endl;
        m_Renderer->RemoveObjectInstance(m_Owner);
    }

    void RenderComponent::SetSprite(std::shared_ptr<Sprite> newSprite, glm::vec3 spritePosition, float opacity, float rotation)
    {
            m_SpriteIndex = newSprite->GetSpriteIndex();
            m_Scale = newSprite->GetScale();
            m_Owner->SetScale(m_Scale);
            isImageSet = true;
            m_SpriteIndex = newSprite->GetSpriteIndex();
            m_Sprite = newSprite;
            m_SpritePosition = spritePosition;
            m_Opacity = opacity;

            glm::vec3 position = m_Owner->GetPosition() + spritePosition;

            //Adds a new object to the render to be renderer
            m_Renderer->AddRender(m_Owner, this->ID, newSprite->GetTextureString(), position, newSprite->GetWidth(), newSprite->GetHeight(), m_Scale, newSprite->GetSpriteSheetDimensions(), newSprite->GetSpriteIndex(), opacity, rotation);


    }

    void RenderComponent::SetTilemap(std::shared_ptr<Tilemap> newTilemap, glm::vec3 tilemapPosition, float rotation)
    {
        isImageSet = true;
        isTilemap = true;
        m_Tilemap = newTilemap;
        m_TilemapPosition = tilemapPosition;
        m_TilemapTexture = 0;

        float fixedGap = 16.f; // Define a fixed gap in world coordinates
        float cumulativeXPosition = (m_Owner->GetPosition().x + tilemapPosition.x); // Start from the owner's X position
        float cumulativeYPosition = (m_Owner->GetPosition().y + tilemapPosition.y); // Start from the owner's X position


        float scaledHeight;

        if (m_Tilemap) {
            for (int row = 0; row < m_Tilemap->GetRows(); ++row) {
                cumulativeXPosition = m_Owner->GetPosition().x + tilemapPosition.x; // Reset X position for new row
                for (int col = 0; col < m_Tilemap->GetCols(); ++col) {
                    auto tile = m_Tilemap->GetTile(row, col);
                    if (tile) {

                        // Set the position based on the cumulative width of all previous tiles
                        glm::vec3 position = glm::vec3(cumulativeXPosition, cumulativeYPosition, 0.0f);
                        glm::vec3 scale = tile->GetScale();
                        float scaledWidth = m_Tilemap->GetTileWidth() * scale.x; // Calculate the scaled width
                        scaledHeight = m_Tilemap->GetTileHeight() * scale.y; // Calculate the scaled height
                        glm::ivec2 spriteSheetDims = glm::ivec2(tile->GetSpriteSheetDimensions());
                        glm::ivec2 spriteIndex = glm::ivec2(tile->GetSpriteIndex());

                        m_Renderer->AddRender(m_Owner, this->ID, tile->GetTextureString(), position, m_Tilemap->GetTileWidth(), m_Tilemap->GetTileHeight(), scale, spriteSheetDims, spriteIndex, 1.0f, rotation);

                        // Update cumulative X position with scaled width and fixed gap
                        cumulativeXPosition += scaledWidth + fixedGap;

                    }
                }
                // After finishing a row, update the Y position for the next row using scaledHeight
                cumulativeYPosition -= (scaledHeight + fixedGap);
            }
        }
    }

    void RenderComponent::UpdateRenderPosition()
    {
        //Update the render instance with the new position, only if the position changes
        if (m_Owner && isImageSet && m_UpdatedRenderPosition != m_Owner->GetPosition()) {
            m_UpdatedRenderPosition = m_Owner->GetPosition();
            if (m_Tilemap != nullptr)
            {
                UpdateTilemap();
            }
            else if (m_Sprite != nullptr)
            {
                m_Renderer->UpdateRender(m_Owner, this->ID, m_UpdatedRenderPosition + m_SpritePosition, m_SpriteIndex, m_Opacity); // Update the render instance with the new position

            }
        }
    }

    void RenderComponent::UpdateTilemap()
    {
        float fixedGap = 16.f; // Define a fixed gap in world coordinates
        float cumulativeXPosition = (m_Owner->GetPosition().x + m_TilemapPosition.x); // Start from the owner's X position
        float cumulativeYPosition = (m_Owner->GetPosition().y + m_TilemapPosition.y); // Start from the owner's X position


        float scaledHeight;

        if (m_Tilemap) {
            for (int row = 0; row < m_Tilemap->GetRows(); ++row) {
                cumulativeXPosition = m_Owner->GetPosition().x + m_TilemapPosition.x; // Reset X position for new row
                for (int col = 0; col < m_Tilemap->GetCols(); ++col) {
                    auto tile = m_Tilemap->GetTile(row, col);
                    if (tile) {
                        // Set the position based on the cumulative width of all previous tiles
                        glm::vec3 position = glm::vec3(cumulativeXPosition, cumulativeYPosition, 0.0f);
                        glm::vec3 scale = tile->GetScale();
                        float scaledWidth = m_Tilemap->GetTileWidth() * scale.x; // Calculate the scaled width
                        scaledHeight = m_Tilemap->GetTileHeight() * scale.y; // Calculate the scaled height
                        glm::ivec2 spriteSheetDims = glm::ivec2(tile->GetSpriteSheetDimensions());
                        glm::ivec2 spriteIndex = glm::ivec2(tile->GetSpriteIndex());

                        m_Renderer->UpdateRender(m_Owner, this->ID, position, spriteIndex);

                        // Update cumulative X position with scaled width and fixed gap
                        cumulativeXPosition += scaledWidth + fixedGap;

                    }
                }
                // After finishing a row, update the Y position for the next row using scaledHeight
                cumulativeYPosition -= (scaledHeight + fixedGap);
            }
        }
    }

    void RenderComponent::UpdateRender(Object* owner, int componentID, glm::vec3 position, glm::ivec2 spriteIndex, float opacity)
    {
    }

  

  



}
