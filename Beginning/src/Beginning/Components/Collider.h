#pragma once

#include "Component.h"
#include "../Physics/Physics.h"
#include <box2d/box2d.h>
#include <vector>
//#include "../Log.h"
#include "RigidbodyComponent.h"
#include "../Events/EventType.h"
#include "../Events/CollisionEvent.h"
#include "../Events/ContactListener.h"

namespace Beginning {
    
    class Physics;
    class Renderer;

    enum class ColliderType {
        Box,
        Circle,
        Polygon
    };

    class ColliderComponent : public Component {
    public:

        // Factory method to create new ColliderComponent instances
        static std::shared_ptr<ColliderComponent> Create(ColliderType colliderType, glm::vec3 initialPosition = glm::vec3(0.0f, 0.0f, 0.0f), float boxWidth = 1.0f, float boxHeight = 1.0f, float radius = 0.5f, const std::vector<glm::vec2>& polygonVertices = std::vector<glm::vec2>{ glm::vec2(0.0f, 0.0f) }, bool render = false) {
            return std::shared_ptr<ColliderComponent>(new ColliderComponent(colliderType, initialPosition, boxWidth, boxHeight, radius, polygonVertices, render));
        }

        void Update(float deltaTime) override; // Implemented
        void Start() override;                 // Implemented
        void Shutdown() override;


        // Method to retrieve vertices for rendering
        std::vector<b2Vec2> GetVertices() const;

        // Method to retrieve the circle radius for rendering
        float GetCircleRadius() const;

        ColliderType GetColliderType() const;

		void SetCircleRadius(float radius);
		void SetBoxSize(float width, float height);


        //COLLISION EVENTS --------------
        void OnCollision(Event* event);
        void OnOverlap(Event* event);


        // Method to bind a custom collision handler
        template<typename T>
        void OnCollisionAddDynamic(T* instance, void(T::* memberFunction)(CollisionEvent&));

        

        bool m_Render = false;

    private:

        ColliderComponent(ColliderType colliderType, glm::vec3 initialPosition = glm::vec3(0.0f, 0.0f, 0.0f), float boxWidth = 1.0f, float boxHeight = 1.0f, float radius = 0.5f, const std::vector<glm::vec2>& polygonVertices = std::vector<glm::vec2>{ glm::vec2(0.0f, 0.0f) }, bool render = false);
        

        // Internal method to simulate collision detection (this could be private if not used externally)
        void SimulateCollision(CollisionEvent& event);

        std::vector<std::function<void(CollisionEvent&)>> collisionHandlers;  // List of collision handlers

        Physics* m_PhysicsEngine;
        b2Body* m_Body;
        b2BodyDef m_BodyDef;
        b2Fixture* m_Fixture;  // Store the fixture to access shape details

        ColliderType m_ColliderType;

		float m_BoxWidth;
		float m_BoxHeight;
        
        float m_Radius;

        const std::vector<glm::vec2>& m_PolygonVertices;
        
        
        glm::vec3 m_InitialPosition;

        friend class Renderer;
		friend class ContactListener;

    };

    template<typename T>
    inline void ColliderComponent::OnCollisionAddDynamic(T* instance, void(T::* memberFunction)(CollisionEvent&))
    {
        // Bind the member function to the instance and store it in the handlers list
        collisionHandlers.push_back([=](CollisionEvent& event) {
            (instance->*memberFunction)(event);
            });

        std::cout << "Handler registered for OnCollision." << std::endl;  // Debugging line
    }

}