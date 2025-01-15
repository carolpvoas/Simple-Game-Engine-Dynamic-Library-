#include "EventHandler.h"

namespace Beginning {

	EventHandler* EventHandler::instance = nullptr;
	Input* EventHandler::m_InputManager = nullptr;

	EventHandler::EventHandler()
	{
		instance = this;
		m_InputManager = new Input();
	}


	EventHandler::~EventHandler()
	{
		delete m_InputManager;
		m_InputManager = nullptr;
	}

	void EventHandler::Update()
	{
		//Checks if user closed the window
		//Then communicates with the event handler to close the application
		SDL_Event event;
		while (SDL_PollEvent(&event) != 0)
		{
			switch (event.type)
			{
			case SDL_QUIT:
				closed = true;
				//TO-DO: Actually close the application
				break;

			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					closed = true;
				}
			}
		}
		SDL_Delay(10);
	}

	void EventHandler::Subscribe(EventType type, std::function<void(Event*)> callback)
	{
		listeners[type].push_back(callback);
	}

	void EventHandler::Dispatch(Event* event)
	{
		EventType type = event->getEventType();
		if (listeners.find(type) != listeners.end()) {
			for (auto& listener : listeners[type]) {
				listener(event);  // Call each registered callback
			}
		}
	}


}

