#pragma once

#include "Component.h"
#include "../Physics/Physics.h"
#include <box2d/box2d.h>
//#include "Collider.h"
#include "RenderComponent.h"

namespace Beginning {

    class Physics;
    class Object;

    class RigidbodyComponent : public Component {
    public:
		//TO-DO: Set code explanation
        //TO-DO: Check const
        ~RigidbodyComponent();

        // Factory method to create new RigidbodyComponent instances
        static std::shared_ptr<RigidbodyComponent> Create(glm::vec3 initialPosition, b2BodyType bodyType, float density = 1.0f, float friction = 0.3f, glm::vec3 scale = glm::vec3(10.f, 10.f, 10.f)) {
            return std::shared_ptr<RigidbodyComponent>(new RigidbodyComponent(initialPosition, bodyType, density, friction, scale));
        }
        
        
        void Start() override;
        void Update(float deltaTime) override;
        void Shutdown() override;
        
        void ApplyForce(const glm::vec2& force);
        void ApplyImpulse(const glm::vec2& impulse);

        void SetLinearVelocity(const glm::vec2& velocity);
        void SetGravityScale(float scale); // Add this method

        //Methods for user to change after creation of the component
        void SetBodyType(b2BodyType bodyType);
        void SetBodyDesinty(float density);
        

        void ActiveRigidbody(bool activate);

    private:

        RigidbodyComponent(glm::vec3 initialPosition, b2BodyType bodyType, float density = 1.0f, float friction = 0.3f, glm::vec3 scale = glm::vec3(10.f, 10.f, 10.f));

        //Because the rigidbody updates the object position with the physics body, we need to update the physics body with the object position, when we want to change the object position 
        void SetBodyPosition(glm::vec3 position);
        
        bool m_Active = false;

        Physics* m_PhysicsEngine;
        b2Body* m_Body;
        b2BodyDef m_BodyDef;
        b2Fixture* m_Fixture;

        b2FixtureDef fixtureDef;

        float m_Density;
        float m_Friction;
        glm::vec3 m_Scale;

        friend class ColliderComponent;
        friend class Object;

    };

}