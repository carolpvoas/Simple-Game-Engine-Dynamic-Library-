#pragma once
#include <vector>
#include "UIObject.h"

namespace Beginning {

    class Application;
    class UIObject;

    //CLASS DEFINITION: It's a class that manages all objects in the game. It's designed to create, update, and delete objects.
    //It's managed by Application.
    class UIManager {
    
    public:
        
        // Factory method to create and register objects
        template<typename T, typename... Args>
        T* CreateUIObject(Args&&... args);

        void ClearObjects();

        void UpdateObjects();

        void RemoveObject(UIObject* object);

    private:
        UIManager() = default;
        ~UIManager() {
            ClearObjects();
        }


        std::vector<UIObject*> m_Elements;

        friend class Application;
    };

    template<typename T, typename... Args>
    T* UIManager::CreateUIObject(Args&&... args) {
        static_assert(std::is_base_of<UIObject, T>::value, "T must inherit from UIObject");

        T* object = new T(std::forward<Args>(args)...);
        m_Elements.push_back(object);  // Automatically add to m_Objects as a raw pointer
        return object;
    }

    
}