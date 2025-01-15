#pragma once

#include "KeyCodes.h"
#include "../Events/EventHandler.h"
#include "../Application.h"
#include "../Objects/Object.h"

namespace Beginning {
    
	class EventHandler;
    class Object;

	//CLASS DEFINITION: Translates the input from the user
    //It's managed by Event Handler.
    class Input {
    public:
        
        static Input& GetInstance() {
            static Input instance;
            return instance;
        };

		//Checks if a key is pressed
        bool KeyPressed(SDL_Scancode key);

        void BindPlayer(Object* object);

    private:
        Input() = default;
        ~Input() = default;
       
        // Delete copy constructor and assignment operator to prevent copying
        Input(const Input&) = delete;
        Input& operator=(const Input&) = delete;

        Object* m_CurrentPlayer;

        //For event handler to create an instance of the event handler, needs to be friend
        friend class EventHandler;
        friend class Application;
        // Additional input methods for other types of input (mouse, gamepad, etc.)
    };

}