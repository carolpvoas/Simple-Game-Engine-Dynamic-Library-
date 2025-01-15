#pragma once

#include "../bgpch.h"

#include "Events/EventHandler.h"
#include "Renderer/Renderer.h"
#include "Physics/Physics.h"
#include "Input/Input.h"
#include "Objects/ObjectManager.h"
#include "UI/UIManager.h"

//CLASS DEFINITION: It's the main class of the engine. It's responsible for running the game loop and manage the core components of the engine. Also, it's the class that the client will inherit from to create the game
//meaning that the user will use some application methods

namespace Beginning {

	class EventHandler;
	class Renderer;
	class Physics;
	class ObjectManager;
	class UIManager;

	class Input;

	class Application
	{
		public:

			Application();
			virtual ~Application();

			//Method to get the singleton instance of the application
			static Application& Get() { return *s_Instance; }
			
			//Used by the client to update the game
			virtual void OnUpdate() = 0;

			//Run the game loop
			void Run();

			// Allow the user to get the input manager and call functions like KeyPressed
			static Input& GetInput();
			
			// Allow the user to call functions like CreateObject
			static ObjectManager& GetObjectManager();

			// Allow the user to call functions like CreateObject
			static UIManager& GetUIManager();


	private:

		bool m_Running = true;

		// Disallow copy and assignment
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		
		//Singleton instance of application
		static Application* s_Instance;
		
		//Core Engine Components
		static EventHandler* m_EventHandler;
		static Renderer* m_Renderer;
		static Physics* m_Physics;
		static ObjectManager* m_ObjectManager;
		static UIManager* m_UIManager;

		


	};
	
	//To be defined in the client, it enables the client to create the application
	Application* CreateApplication();


	

	
}

