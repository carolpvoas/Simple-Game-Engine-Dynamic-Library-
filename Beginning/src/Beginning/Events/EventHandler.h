#pragma once
#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "../../bgpch.h"
#include "../Application.h"
#include <map>
#include <typeindex>
#include <any>

#include "../Input/Input.h"
#include "EventType.h"

namespace Beginning
{
    class Application;
    class Input;

	//CLASS DEFINITION: Handles all events that occur in the application.
	//It's managed by Application.
    class EventHandler
    {
    public: 

        static EventHandler& GetInstance() {
            return *instance;
        };

        // Expose Input Manager as a const pointer for user use
        static Input* GetInputManager() { return m_InputManager; }

        void Subscribe(EventType type, std::function<void(Event*)> callback);

        void Dispatch(Event* event);

    private:
        EventHandler();
        ~EventHandler();

        void Update();

        // Delete copy constructor and assignment operator to prevent copying
        EventHandler(const EventHandler&) = delete;
        EventHandler& operator=(const EventHandler&) = delete;

        //Input Manager Instance
        static Input* m_InputManager;

        static EventHandler* instance;

        //Gets the value from the input manager, which defines if application should close
        bool closed = false;

        // Map of event types to their listeners (callbacks)
        std::map<EventType, std::vector<std::function<void(Event*)>>> listeners;

		//For application to create an instance of the event handler, needs to be friend
		friend class Application;

        
    };

   
}

#endif // EVENTHANDLER_H