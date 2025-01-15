#pragma once
#include "UIObject.h"
#include <string>
#include <SDL.h>

namespace Beginning {

    class Sprite;

    class UIImage : public UIObject {

    public:

        void SetImage(std::shared_ptr<Sprite> newSprite, glm::vec3 spritePosition = glm::vec3(0.0f, 0.0f, 0.0f), float opacity = 1.0f, float rotation = 0.0f);

        virtual void UpdateRender(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2 size = glm::vec2(0.0f, 0.0f), bool isVisible = true) override;

    protected:

        UIImage();
        virtual ~UIImage();

       // std::string text;
        friend class UIManager;


    };

}
