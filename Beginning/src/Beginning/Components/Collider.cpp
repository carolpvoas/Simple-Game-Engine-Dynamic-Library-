#include "Collider.h"
#include "../Events/EventType.h"

namespace Beginning {

    ColliderComponent::ColliderComponent(ColliderType colliderType, glm::vec3 initialPosition, float boxWidth, float boxHeight, float radius, const std::vector<glm::vec2>& polygonVertices, bool render)
        : m_PhysicsEngine(&Physics::GetInstance()), m_ColliderType(colliderType), m_BoxWidth(boxWidth), m_BoxHeight(boxHeight), m_Radius(radius), m_PolygonVertices(polygonVertices), m_InitialPosition(initialPosition), m_Render(render){
        
        // Subscribe to Collision events
        EventHandler::GetInstance().Subscribe(EventType::Collision,
            [this](Event* event) { OnCollision(event); });

        // Subscribe to Overlap events
        EventHandler::GetInstance().Subscribe(EventType::Overlap,
            [this](Event* event) { OnOverlap(event); });
    }

    void ColliderComponent::Update(float deltaTime)
    {
        
    }

    void ColliderComponent::Start()
    {
        m_InitialPosition = (m_Owner->GetPosition() + m_InitialPosition) / 1000.f;
        m_BodyDef.position.Set(m_InitialPosition.x, m_InitialPosition.y);
        //m_Body = m_PhysicsEngine->GetInstance()->GetWorld()->CreateBody(&m_BodyDef);
       
		//Check if the object has a RigidbodyComponent, if not, create an exception
        if(!m_Owner->GetComponent<Beginning::RigidbodyComponent>())
        { 
           // BG_ERROR("The Object Doesn't Own A RigidBody Component - Collider.h");
            std::terminate();
            //return;
        }
        else
        {
			m_Body = m_Owner->GetComponent<Beginning::RigidbodyComponent>()->m_Body;
        }

        b2FixtureDef fixtureDef;

        switch (m_ColliderType) {
        case ColliderType::Box: {
            b2PolygonShape dynamicBox;
            dynamicBox.SetAsBox(m_BoxWidth, m_BoxHeight);
            fixtureDef.shape = &dynamicBox;
            m_Fixture = m_Body->CreateFixture(&fixtureDef);
            if (m_Owner->GetComponent<Beginning::ColliderComponent>()->m_Render == true)
            {

            }
            break;
        }
        case ColliderType::Circle: {
            b2CircleShape circleShape;
            circleShape.m_radius = m_Radius;
            fixtureDef.shape = &circleShape;
            m_Fixture = m_Body->CreateFixture(&fixtureDef);
            break;
        }
        case ColliderType::Polygon: {
            b2PolygonShape polygonShape;
            std::vector<b2Vec2> vertices(m_PolygonVertices.size());
            for (size_t i = 0; i < m_PolygonVertices.size(); ++i) {
                vertices[i].Set(m_PolygonVertices[i].x, m_PolygonVertices[i].y);
            }
            polygonShape.Set(vertices.data(), vertices.size());
            fixtureDef.shape = &polygonShape;
            break;
        }
        }
    }

    void ColliderComponent::Shutdown()
    {
    }


    std::vector<b2Vec2> ColliderComponent::GetVertices() const {
        std::vector<b2Vec2> vertices;
        if (m_ColliderType == ColliderType::Box || m_ColliderType == ColliderType::Polygon) {
            b2PolygonShape* polygonShape = (b2PolygonShape*)m_Fixture->GetShape();
            for (int i = 0; i < polygonShape->m_count; i++) {
                vertices.push_back(polygonShape->m_vertices[i]);
            }
        }
        return vertices;
    }

    float ColliderComponent::GetCircleRadius() const {
        if (m_ColliderType == ColliderType::Circle) {
            b2CircleShape* circleShape = (b2CircleShape*)m_Fixture->GetShape();
            return circleShape->m_radius;
        }
        return 0.0f;
    }

    ColliderType ColliderComponent::GetColliderType() const {
        return m_ColliderType;
    }

    void ColliderComponent::SetCircleRadius(float radius)
    {
         m_Fixture->GetShape()->m_radius = radius;
    }
    
    void ColliderComponent::OnCollision(Event* event) {
        CollisionEvent* collisionEvent = dynamic_cast<CollisionEvent*>(event);
    }

    void ColliderComponent::OnOverlap(Event* event) {
        OverlapEvent* overlapEvent = dynamic_cast<OverlapEvent*>(event);
    }

    void ColliderComponent::SimulateCollision(CollisionEvent& event)
    {
        // Call all bound collision handlers
        for (auto& handler : collisionHandlers) {
            handler(event);

            std::cout << "Handler called." << std::endl;  // Debugging line
        }

       
    }


    
}
