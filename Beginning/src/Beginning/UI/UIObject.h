#pragma once

#include <glm.hpp>
#include "UIManager.h"


namespace Beginning{

    class Renderer;

    enum class UIAlignment {
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight,
        Center
    };

    class UIObject {
    public:

        // Method to update the UI element (optional for animated UI)
        virtual void Update(float deltaTime);

		virtual void UpdateRender(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2 size = glm::vec2(0.0f,0.0f), bool isVisible = true) {};

        // Position, size, visibility, etc.
        bool IsVisible = true;
        //int x, y, width, height;
        
        /*// Set position
        void SetPosition(int xPos, int yPos) {
            x = xPos;
            y = yPos;
        }

        // Set size
        void SetSize(int w, int h) {
            width = w;
            height = h;
        }*/

        // Set the alignment and calculate position based on screen size
        void SetAlignment(UIAlignment newAlignment, glm::vec3 offset);
       

    protected:

        UIObject();
        ~UIObject();
        Renderer* m_Renderer;

        glm::vec2 m_Size;

		bool currentVisibility = true;

        glm::vec3 currentPosition;

    private:

        virtual void CleanUIObject();
        friend class UIManager;


    };

}