#include "RigidbodyComponent.h"
#include "../Objects/Object.h"

namespace Beginning {
    
    const float PIXELS_PER_METER = 100.0f;

    RigidbodyComponent::RigidbodyComponent(glm::vec3 initialPosition, b2BodyType bodyType, float density, float friction, glm::vec3 scale)
        : m_PhysicsEngine(&Physics::GetInstance()), m_Body(nullptr), m_Density(density), m_Friction(friction) 
    {
        m_BodyDef.type = bodyType;
        m_BodyDef.position.Set(initialPosition.x, initialPosition.y);
        m_Scale = scale;
    }

    RigidbodyComponent::~RigidbodyComponent()
    {
    }


    void RigidbodyComponent::Start() {
        
        RigidbodyComponent* rigidbodyComponent = dynamic_cast<RigidbodyComponent*>(this);
        m_Owner->AddRigidbodyComponent(rigidbodyComponent);

        //Creates physics' body
        m_Body = m_PhysicsEngine->world->CreateBody(&m_BodyDef);
        m_Body->GetUserData().pointer = reinterpret_cast<uintptr_t>(m_Owner);
        
        if (m_Body == nullptr)
        {
			std::cout << "Body is null" << std::endl;
			return;
        }
        
		//Creates the physics' object shape
        b2PolygonShape shape;
        shape.SetAsBox(m_Owner->m_Scale.x, m_Owner->m_Scale.y);
        shape.SetAsBox(m_Scale.x, m_Scale.y);

		//Sets the shape and its properties
        fixtureDef.shape = &shape;
        fixtureDef.density = m_Density;
        fixtureDef.friction = m_Friction;

        // Collision filtering: this fixture won't collide with anything, if user wants collision it needs to add the collider component
        b2Filter filter;
        filter.categoryBits = 0x0001;
        filter.maskBits = 0x0000; // No collisions with any other category
        fixtureDef.filter = filter;

        m_Fixture = m_Body->CreateFixture(&fixtureDef);

        if (m_Fixture == nullptr) {
            std::cerr << "Failed to create fixture" << std::endl;
            return;
        }

        std::cout << "Fixture created successfully" << std::endl;

    } 

    void RigidbodyComponent::Update(float deltaTime) {
        if (m_Active)
        {
            if (m_Owner) {
                b2Vec2 position = m_Body->GetPosition();
                
                //Updates the object's position according to the physics' body
                if (m_Body->GetType() != b2_staticBody)
                {
                    m_Owner->SetPosition(glm::vec3(position.x * PIXELS_PER_METER, position.y * PIXELS_PER_METER, 0.0f));
                }

            }
        }
    }

    void RigidbodyComponent::Shutdown()
    {
        m_PhysicsEngine->bodiesToDestroy.push_back(m_Body);

    }
    
    void RigidbodyComponent::ApplyForce(const glm::vec2& force) {
        m_Body->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
    }

    void RigidbodyComponent::ApplyImpulse(const glm::vec2& impulse) {
        m_Body->ApplyLinearImpulseToCenter(b2Vec2(impulse.x, impulse.y), true);
    }

    void RigidbodyComponent::SetLinearVelocity(const glm::vec2& velocity)
    {
        if (m_Body)
        {
            m_Body->SetAwake(true);
            m_Body->SetLinearVelocity(b2Vec2(velocity.x / PIXELS_PER_METER, velocity.y / PIXELS_PER_METER));
        }
        //   std::cout << "Set Linear Velocity: " << m_Body->GetLinearVelocity().x << std::endl; // Add this
    }

    void RigidbodyComponent::SetGravityScale(float scale) {
        m_Body->SetGravityScale(scale);
        //  std::cout << "Gravity scale set to: " << scale << std::endl;
    }

    void RigidbodyComponent::SetBodyType(b2BodyType bodyType)
    {
        m_Body->SetType(bodyType);
    }

    void RigidbodyComponent::SetBodyDesinty(float density)
    {
        // Loop through all fixtures in the body
        for (b2Fixture* fixture = m_Body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
            fixture->SetDensity(5.0f);  // Set the new density for the fixture
        }

        // Recalculate the body's mass properties based on the new density
        m_Body->ResetMassData();
    }

    void RigidbodyComponent::ActiveRigidbody(bool activate)
    {
        m_Active = activate;
    }

    void RigidbodyComponent::SetBodyPosition(glm::vec3 position)
    {
        m_Body->SetTransform(b2Vec2(position.x / 100.f, position.y / 100.f), m_Body->GetAngle());
    }


}