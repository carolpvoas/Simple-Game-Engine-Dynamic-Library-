#include "Physics.h"
#include <box2d/box2d.h>
#include "../../bgpch.h"


namespace Beginning{
	Physics* Physics::instance = nullptr;

	Physics::Physics() 
	{
		contactListener = new ContactListener();

		instance = this;

		//World physics' settings
		b2Vec2 gravity(0.0f, -9.81f); // Define gravity (in meters per second squared, downward)
	
		world = new b2World(gravity);
		world->SetAllowSleeping(true); // Enable sleeping (inactive bodies are put to sleep to save CPU)
		world->SetContinuousPhysics(true); // Enable continuous physics (improves collision handling for fast-moving objects)
		world->SetContactListener(contactListener);
	}

	Physics::~Physics() {
		delete contactListener;
		delete world;
	}

	void Beginning::Physics::OnUpdate()
	{
		if (world == nullptr) return;

		//Physics update
		float timeStep = 1.0f / 60.0f; // Time step (in seconds)
		int32 velocityIterations = 6; // Number of velocity iterations for the solver
		int32 positionIterations = 2; // Number of position iterations for the solver

		world->Step(timeStep, velocityIterations, positionIterations); // Update the physics simulation

		for (b2Body* body : bodiesToDestroy) {
			if (body != nullptr)
			{
				world->DestroyBody(body);
			}
			
		}
		bodiesToDestroy.clear();


	}
}