#include "ContactListener.h"

namespace Beginning {

    const float MIN_HIT_VELOCITY = 5.0f;

    void ContactListener::BeginContact(b2Contact* contact)
    {

        // This function is called when two fixtures begin to touch
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();

        // Check if they intersect using user data or other logic
        b2Body* bodyA = fixtureA->GetBody();
        b2Body* bodyB = fixtureB->GetBody();

        // Set user data to point to your game object
        //bodyA->GetUserData().pointer = reinterpret_cast<uintptr_t>(fixtureA);
        //bodyB->GetUserData().pointer = reinterpret_cast<uintptr_t>(fixtureB);

        void* userDataA = reinterpret_cast<void*>(bodyA->GetUserData().pointer);
        void* userDataB = reinterpret_cast<void*>(bodyB->GetUserData().pointer);
        // Get user data from bodies to identify the objects
        //uintptr_t userDataA = bodyA->GetUserData().pointer;
        //uintptr_t userDataB = bodyB->GetUserData().pointer;

        if (userDataA && userDataB) {
            // Handle the collision, e.g., determine if this is a "hit"
            // You can also check if these bodies have specific properties that should result in a "hit"
            // Check if either fixture is a sensor (trigger)
            bool isFixtureATrigger = fixtureA->IsSensor();
            bool isFixtureBTrigger = fixtureB->IsSensor();

            if (isFixtureATrigger || isFixtureBTrigger) {
                // Create an overlap event
                OverlapEvent overlapEvent(userDataA, userDataB);
                // Dispatch the overlap event
                EventHandler::GetInstance().Dispatch(&overlapEvent);

               
            }
            else {
                // Create a collision event
                CollisionEvent collisionEvent(userDataA, userDataB);
                // Dispatch the collision event
                EventHandler::GetInstance().Dispatch(&collisionEvent);
                //std::cout << "Collision event dispatched." << std::endl;

                // Assuming userDataA and userDataB are pointers to game objects
                Object* objectA = static_cast<Object*>(userDataA);
                Object* objectB = static_cast<Object*>(userDataB);

                if (objectA && objectA->GetComponent<ColliderComponent>()) {
                   // std::cout << "Calling SimulateCollision on object A." << std::endl;
                    objectA->GetComponent<ColliderComponent>()->SimulateCollision(collisionEvent);
                }
                else {
                    //std::cout << "Collider component not found on object A." << std::endl;
                }

                if (objectB && objectB->GetComponent<ColliderComponent>()) {
                    //std::cout << "Calling SimulateCollision on object B." << std::endl;
                    objectB->GetComponent<ColliderComponent>()->SimulateCollision(collisionEvent);
                }
                else {
                   // std::cout << "Collider component not found on object B." << std::endl;
                }

            }

           
        }

    }
    ContactListener::~ContactListener()
    {
    }
}