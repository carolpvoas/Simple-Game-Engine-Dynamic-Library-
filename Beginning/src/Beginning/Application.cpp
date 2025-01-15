#include "Application.h"

#include <SDL.h>
#include "glad/glad.h"

namespace Beginning {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	// Initialize the static member
	Application* Application::s_Instance = nullptr;
	EventHandler* Application::m_EventHandler = nullptr;
	Renderer* Application::m_Renderer = nullptr;
	Physics* Application::m_Physics = nullptr;
	ObjectManager* Application::m_ObjectManager = nullptr;
	UIManager* Application::m_UIManager = nullptr;

	float deltaTime = 0.016f;

	Application::Application()
	{
		//Initializes the core components of the Game Engine
		m_EventHandler = new EventHandler();
		m_Renderer = new Renderer();
		m_Physics = new Physics();
		m_ObjectManager = new ObjectManager();
		m_UIManager = new UIManager();

		m_Renderer->InitRenderer(600, 600);

	}
		

	Application::~Application()
	{
		delete m_EventHandler;
		m_EventHandler = nullptr;

		delete m_Renderer;
		m_Renderer = nullptr;

		delete m_Physics;
		m_Physics = nullptr;

		m_ObjectManager->ClearObjects();
		delete m_ObjectManager;
		m_ObjectManager = nullptr;

		m_UIManager->ClearObjects();
		delete m_UIManager;
		m_UIManager = nullptr;

		delete s_Instance;
		s_Instance = nullptr;
		SDL_Quit();

	}


	void Application::Run()
	{
		if (this == nullptr) return;
		while (m_Running)
		{
			m_EventHandler->Update();

			//Checks if the user closed the window (ended the application)
			if (m_EventHandler->closed)
			{
				//SDL_Quit();
				break;
			}

			for (int i = 0; i < m_ObjectManager->m_Objects.size(); i++)
			{
				if (m_ObjectManager->m_Objects[i]->isPendingKill)
				{
					//m_ObjectManager->m_Objects.erase(m_ObjectManager->m_Objects.begin() + i);
					m_ObjectManager->RemoveObject(m_ObjectManager->m_Objects[i]);
				}
			}

			OnUpdate();

			m_Renderer->Update();
			m_Physics->OnUpdate();
			m_ObjectManager->UpdateObjects();
			m_UIManager->UpdateObjects();
			

		}
			
	}

	Input& Application::GetInput()
	{
		return *m_EventHandler->GetInputManager();
	}

	ObjectManager& Application::GetObjectManager()
	{
		return *m_ObjectManager;
	}

	UIManager& Application::GetUIManager()
	{
		return *m_UIManager;
	}



}
