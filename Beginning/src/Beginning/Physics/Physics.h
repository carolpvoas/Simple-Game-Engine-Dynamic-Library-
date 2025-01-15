#pragma once

#include <box2d/box2d.h>
#include "../../bgpch.h"
#include "../Application.h"
#include "../Events/ContactListener.h"

namespace Beginning {

    class Application;
	class ContactListener;
	//CLASS DEFINITION: Responsabile for the physics simulation of the game
    //It's managed by Application.
    class Physics {

    public:

		static Physics& GetInstance() {
			return *instance;
		}

        void OnUpdate();

        b2World* world;
        
        std::vector<b2Body*> bodiesToDestroy;


    private:
       
        Physics();
        ~Physics();
        
        static Physics* instance;


        Physics(const Physics&) = delete;
        Physics& operator=(const Physics&) = delete;

        ContactListener* contactListener;

		//For application to create an instance of the physics, needs to be friend
		friend class Application;

    };



}

