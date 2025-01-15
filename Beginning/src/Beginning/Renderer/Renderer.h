#pragma once
#ifndef OPENGL_SDL_RENDERER_H
#define OPENGL_SDL_RENDERER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "../../bgpch.h"
#include "stb_image.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "../Objects/Object.h"
#include "../UI/UIObject.h"
#include "../UI/UIImage.h"
#include "../UI/UIText.h"

#include "../Application.h"

namespace Beginning{

    class Application;
    class Object;
    class UIObject;
    class UIImage;
    class UIText;

	//CLASS DEFINITION: It's responsable for all the rendering in the application.
	//It's managed by Application.
    class Renderer {

    public:
        
        //Struct to hold the object data
        struct ObjectStruct {
            Object* m_Owner;

            int componentID;

            unsigned int texture;
            std::string textureName;
            glm::vec3 position;
            glm::vec3 scale;

            //sprite sheet dimensions, for you to choose your sprite
            glm::ivec2 spriteSheetDims;
            glm::ivec2 spriteIndex;

            int width;
            int height;

            float rotation;

            float opacity;

            bool hasAnimator;

            //animation framerate
            Uint32 lastFrameTime;
            float frameDuration;
            int currentFrame;
        };

        struct UIImageStruct {
            UIImage* m_Owner;

            unsigned int texture;
            std::string textureName;
            glm::vec3 position;
            glm::vec3 scale;

            glm::ivec2 spriteSheetDims;
            glm::ivec2 spriteIndex;

            int width;
            int height;

            float rotation;

            float opacity;

            bool isVisible;
        };

		struct UITextStruct {
			UIText* m_Owner;
			
            unsigned int texture;
			int size;
            glm::vec3 position;

            bool isVisible;
		};

	    //Instance of the singleton Renderer
        static Renderer& GetInstance() {
            return *instance;
        }


	    //Initialize the renderer, creates the window and the OpenGL context
        int InitRenderer(int screenWidth, int screenHeight);
	
	    //Sets the vertex data of a rectangle
        int InitVertexData();
        int InitVertexDataText();

	    //Creates a shader program
        int ShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);

        //Creates a shader program for text
        int TextShaderProgram(const char* vertexShaderSourceText, const char* fragmentShaderSourceText);

        //Renderer loop
        void Update();

        //Update the render instance with the new position
        void UpdateRender(Object* owner, int componentID, glm::vec3 position = glm::vec3(0.f, 0.f, 0.f), glm::ivec2 spriteIndex = glm::ivec2(0, 0), float opacity = 1.0f);

		void UpdateUIRender(UIImage* owner, glm::vec3 position, int width, int height, bool isVisible);

	    //Loads a texture from a file
        unsigned int LoadTexture(std::string fileName, Object* owner);
        unsigned int LoadText(TTF_Font* font, std::string text, glm::vec4 color);

        //Adds an object to be rendered
        void AddRender(Object* owner, int componentID, std::string textureName, glm::vec3 position, int width, int height, glm::vec3 scale, glm::ivec2 spriteSheetDims, glm::ivec2 spriteIndex, float opacity = 1.0f, float rotation = 0.0f);

        //Adds an object to be rendered
        void AddUIRender(UIImage* owner, std::string textureName, glm::vec3 position, int width, int height, glm::vec3 scale, glm::ivec2 spriteSheetDims, glm::ivec2 spriteIndex, float opacity = 1.0f, bool isVisible = true, float rotation = 0.0f);

        void AddTextRender(UIText* owner, std::string text, glm::vec4 color, TTF_Font* font, int size, glm::vec3 position);

		//Removes an object from the render list
        void RemoveObjectInstance(Object* object);

        //Stores the texture ID
        unsigned int m_textureID;

        //Stores the objects to be rendered
        std::vector<ObjectStruct> objectInstances;
        std::vector<UIImageStruct> imageInstances;
        std::vector<UITextStruct> textInstances;

    private:
        Renderer();
        ~Renderer();

        static Renderer* instance;

        //Window pointer
        SDL_Window* m_window;

        //Context pointer
        SDL_GLContext m_glContext;
   
	    //Vertex buffer object, element buffer object and vertex array object for the rectangle
        unsigned int m_vboRect;
        unsigned int m_eboRect;
        unsigned int m_vaoRect;

        unsigned int m_vbo;
        unsigned int m_ebo;
        unsigned int m_vao;

        int m_textWidth;
		int m_textHeight;

        //Shader program
        unsigned int m_shaderProgram;
        unsigned int m_shaderProgramText;

        //Instance of the singleton Renderer
        static Renderer* s_Instance;

        //For application to create an instance of the renderer, needs to be friend
        friend class Application;

    };

    extern const char* vertexShaderSourceSprite;
    extern const char* fragmentShaderSourceSprite;


}

#endif