#pragma once
#include <vector>
#include <glm.hpp>
#include <typeinfo>

#include "ObjectManager.h"
#include "../Components/Component.h"
#include "../Components/RenderComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/Collider.h"
#include "../Components/AnimatorComponent.h"

#include "../Application.h"

namespace Beginning {

    class Component;
    class RenderComponent;
    class RigidbodyComponent;
    class Application;

    //CLASS DEFINITION: It's a base class for all objects in the game. It's designed to have components that can be added to it.
    //It's managed by ObjectManager.
    class Object {
    public:
		
		//When a user creates an object, it should override this method to update the object
		virtual void OnUpdate(float deltaTime) {};

        void UpdateComponents(float deltaTime);

        //Adds a component to the object
        void AddComponent(std::shared_ptr<Component> component);

        //Get Component of the object
        template <typename T>
        T* GetComponent();

        //Remove Component from the object
        template <typename T>
        void RemoveComponent();

        //Set the position of the object
        void SetPosition(const glm::vec3& position);
        
        //Get the position of the object
        glm::vec3 GetPosition() { return m_Position; }

        //Gives access to its render component
        void AddRenderComponent(RenderComponent* renderComponent);

        //Gives access to its rigidbody component
        void AddRigidbodyComponent(RigidbodyComponent* rigidbodyComponent);

        void SetScale(glm::vec3 scale);
        glm::vec3 m_Scale;

        bool isPendingKill = false;

        std::string Tag;

    protected:

        Object();
        virtual ~Object();

        bool m_canUpdate = false;

    private:

        virtual void Clean();

        // Store all components of the object
		std::vector<std::shared_ptr<Component>> m_Components;

        std::vector<RenderComponent*> m_RenderComponents;
        RigidbodyComponent* m_RigidbodyComponent;

        glm::vec3 m_Position;
        
       
		

    private:
        //For Object Manager to create object instances, needs to be friend
		friend class ObjectManager;
        friend class Application;

        //friend class RenderComponent;
    };

    template <typename T>
    T* Object::GetComponent() {
        for (auto component : m_Components) {
            if (T* castedComponent = dynamic_cast<T*>(component.get())) {
                return castedComponent;
            }
        }
        return nullptr;
    };

    template <typename T>
    void Object::RemoveComponent() {
        auto it = std::remove_if(m_Components.begin(), m_Components.end(), [](std::shared_ptr<Component>& component) {
            return dynamic_cast<T*>(component.get()) != nullptr;
            });

        if (it != m_Components.end()) {
			it->get()->Shutdown();
            m_Components.erase(it); // The shared_ptr will manage the memory cleanup automatically
            
        }
    }
    

}
