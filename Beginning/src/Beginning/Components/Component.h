#pragma once
namespace Beginning {

    class Object; // Forward declaration

    class Component {
    public:
       
        virtual void Update(float deltaTime) = 0;

        virtual void Start() = 0;

        virtual void Shutdown() = 0;

        void SetOwner(Object* owner) {
           m_Owner = owner;
        }

    protected:
        Object* m_Owner = nullptr;

        ~Component() = default;
    };

}