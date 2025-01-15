#pragma once

#include "Component.h"

#include "../Renderer/Renderer.h"
#include <glm.hpp>
#include <string>
#include <vector>

#include "../Renderer/Sprite.h"
#include "../Renderer/Tilemap.h"

namespace Beginning {
    
    class Renderer;

    //CLASS DEFINITION: It's responsible for communicating with the Renderer and Render the owner object
    class RenderComponent : public Component, public std::enable_shared_from_this<RenderComponent> {

    public:
        static int GlobalID;
        int ID;

        ~RenderComponent();

        // Factory method to create new RenderComponent instances
        static std::shared_ptr<RenderComponent> Create() {
            return std::shared_ptr<RenderComponent>(new RenderComponent());
        }

        void Start() override;
        void Update(float deltaTime) override;
		void Shutdown() override;

        //Set the sprite of the object
        void SetSprite(std::shared_ptr<Sprite> newSprite, glm::vec3 spritePosition = glm::vec3(0.0f, 0.0f, 0.0f), float opacity = 1.0f, float rotation = 0.0f);

		//Set the tilemap of the object
        void SetTilemap(std::shared_ptr<Tilemap> newTilemap, glm::vec3 tilemapPosition = glm::vec3(0.0f, 0.0f, 0.0f), float rotation = 0.0f);

        void UpdateRenderPosition();

        void UpdateTilemap();

        //Update the render instance with the new position
        void UpdateRender(Object* owner, int componentID, glm::vec3 position = glm::vec3(0.f, 0.f, 0.f), glm::ivec2 spriteIndex = glm::ivec2(0, 0), float opacity = 1.0f);
        
    protected:
        

    private:

        RenderComponent();

        Renderer* m_Renderer;
        std::string m_Image;
        glm::vec3 m_Scale;
        glm::ivec2 m_SpriteSheetDims;
        unsigned int m_Texture;
        float m_Opacity;


        std::shared_ptr<Sprite> m_Sprite;
        glm::ivec2 m_SpriteIndex;
        glm::vec3 m_SpritePosition;
        std::shared_ptr<Tilemap> m_Tilemap;
        glm::vec3 m_TilemapPosition;
        unsigned int m_TilemapTexture;


        glm::vec3 m_UpdatedRenderPosition = glm::vec3(0.0f, 0.0f, 0.0f);

        bool isImageSet = false;

        bool isTilemap = false;

     //   friend class Object;
      
    };



}