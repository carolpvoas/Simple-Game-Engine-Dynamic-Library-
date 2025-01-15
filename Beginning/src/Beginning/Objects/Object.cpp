#include "Object.h"


namespace Beginning {

    void Object::UpdateComponents(float deltaTime)
    {
        for (auto component : m_Components) {
            component->Update(deltaTime);
            //std::cout << "Component Updated: " << typeid(*component).name() << std::endl; // Add this
        }
    }

    void Object::AddComponent(std::shared_ptr<Component> component) {
       
        component->SetOwner(this);
        m_Components.push_back(component);
        component->Start();

    }

    void Object::SetPosition(const glm::vec3& position)
    {
        m_Position = position;

        if (m_RigidbodyComponent && m_RigidbodyComponent->m_Active == false)
        {
            m_RigidbodyComponent->SetBodyPosition(m_Position);
        }

        // Loop through all components
        for (auto& component : m_RenderComponents) {

			// Update RenderComponent positions if new position is set
            if (component) {
                component->UpdateRenderPosition();
            }

            // Here you can add more specific updates for other types of components if needed
        }
    }

    void Object::AddRenderComponent(RenderComponent* renderComponent)
    {
        m_RenderComponents.push_back(renderComponent);
    }

    void Object::AddRigidbodyComponent(RigidbodyComponent* rigidbodyComponent)
    {
        m_RigidbodyComponent = rigidbodyComponent;
    }

    void Object::SetScale(glm::vec3 scale)
    {
        m_Scale = scale;
    }

    Object::Object(): m_Position(glm::vec3(0.0f,0.0f,0.0f))
    {
    }

    Object::~Object() {
		
    }
    void Object::Clean()
    {
        for (auto& component : m_Components) {

            component->Shutdown();
            // Here you can add more specific updates for other types of components if needed
        }
        for (auto& component : m_RenderComponents) {

            component->Shutdown();
        }
        if (m_RigidbodyComponent != nullptr)
        {
			//m_RigidbodyComponent->Shutdown();
        }
        m_RenderComponents.clear();
		m_RigidbodyComponent = nullptr;
        m_Components.clear();
    }
}