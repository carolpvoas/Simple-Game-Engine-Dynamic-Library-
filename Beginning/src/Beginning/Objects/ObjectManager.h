#pragma once
#include <vector>
#include "Object.h" 
#include "../Application.h"

namespace Beginning {
    
    class Object;
    class Application;

	//CLASS DEFINITION: It's a class that manages all objects in the game. It's designed to create, update, and delete objects.
	//It's managed by Application.
    class ObjectManager {
    public:

        void DestroyObject(Object* object);

        // Clear all objects
        void ClearObjects();

        // Update all objects
        void UpdateObjects();

        // Factory method to create and register objects
        template<typename T, typename... Args>
        T* CreateObject(Args&&... args);

    private:
        ObjectManager() = default;
        ~ObjectManager() {
            ClearObjects();
        }

        // Remove and delete an object
        void RemoveObject(Object* object);

        std::vector<Object*> m_Objects;  // Store all game objects

        //For application to create an instance of the object manager, needs to be friend
        friend class Application;
    };

    template<typename T, typename... Args>
    T* ObjectManager::CreateObject(Args&&... args) {
        static_assert(std::is_base_of<Object, T>::value, "T must inherit from Object");

        T* object = new T(std::forward<Args>(args)...);
        m_Objects.push_back(object);  // Automatically add to m_Objects as a raw pointer
        object->m_canUpdate = true;
        return object;
    }

}