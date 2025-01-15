#pragma once

#include "Component.h"
#include "Animation.h"
#include "RenderComponent.h"
#include <vector>
#include <string>

namespace Beginning {

    class AnimatorComponent : public Component {
    public:
        
        // Factory method to create new AnimatorComponent instances
        static std::shared_ptr<AnimatorComponent> Create() {
            return std::shared_ptr<AnimatorComponent>(new AnimatorComponent());
        }

        void Update(float deltaTime) override; // Implemented
        void Start() override;                 // Implemented
        void Shutdown() override;

        void AddAnimation(const Animation& animation);
        void SetAnimation(const std::string& name);
        void PlayAnimation(const std::string& name);
        void StopAnimation();

        glm::ivec2 GetCurrentFrame() const;
        float GetFrameDuration() const;
        Animation* GetCurrentAnimation();
        int GetAnimationCount() const { return m_Animations.size(); };

        bool IsAnimated() const;

        bool canPlayAnimation();

        bool IsAnimationPlaying(const std::string& name);

    private:

        std::vector<Animation> m_Animations;
        Animation* m_CurrentAnimation;
        glm::ivec2 m_CurrentFrame;
        float m_FrameTime;

        bool canPlay = false;

    protected:

        AnimatorComponent();

    };

}
