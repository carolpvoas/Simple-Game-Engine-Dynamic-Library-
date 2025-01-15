#pragma once
#include "UIObject.h"
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

namespace Beginning {

    class UIText : public UIObject {

    public:

		void SetText(std::string text, glm::vec4 color, TTF_Font* font, int size, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));

        // Override Update method (if necessary)
        virtual void Update(float deltaTime) override;

    protected:
		UIText();
		virtual ~UIText();


    private:
        std::string text;
        SDL_Color color;
        TTF_Font* font;

        friend class UIManager;
    };

}
