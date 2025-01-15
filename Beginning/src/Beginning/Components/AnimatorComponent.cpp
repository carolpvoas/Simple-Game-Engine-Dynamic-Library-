#include "AnimatorComponent.h"
#include <iostream>

namespace Beginning {

    

    AnimatorComponent::AnimatorComponent()
        : m_CurrentAnimation(nullptr), m_FrameTime(0.0f), m_CurrentFrame(0,0){

    }

    void AnimatorComponent::Start() {
        // Initialize the first animation if available
        if (!m_Animations.empty()) {
            m_CurrentAnimation = &m_Animations[0];
            m_CurrentFrame = m_CurrentAnimation->startFrame;
        }
    }

    void AnimatorComponent::Shutdown()
    {
    }

    void AnimatorComponent::Update(float deltaTime) {
        if (m_CurrentAnimation) {
            m_FrameTime += deltaTime;

            if (m_FrameTime >= m_CurrentAnimation->frameDuration) {
                m_FrameTime = 0.0f;

                // Check if animation should go in reverse
                if (m_CurrentAnimation->startFrame.x > m_CurrentAnimation->endFrame.x) {
                    m_CurrentFrame.x--;

                    // Check if we need to wrap to the previous row
                    if (m_CurrentFrame.x < m_CurrentAnimation->endFrame.x) {
                        m_CurrentFrame.x = m_CurrentAnimation->startFrame.x;
                        m_CurrentFrame.y--;
                    }

                    // Handle the end of the animation
                    if (m_CurrentFrame.y < m_CurrentAnimation->endFrame.y) {
                        if (m_CurrentAnimation->loop) {
                            m_CurrentFrame = m_CurrentAnimation->startFrame;
                        }
                        else {
                            m_CurrentFrame = m_CurrentAnimation->endFrame;
                        }
                    }
                }
                else { // Normal forward animation
                    m_CurrentFrame.x++;

                    // Check if we need to wrap to the next row
                    if (m_CurrentFrame.x > m_CurrentAnimation->endFrame.x) {
                        m_CurrentFrame.x = m_CurrentAnimation->startFrame.x;
                        m_CurrentFrame.y++;
                    }

                    // Handle the end of the animation
                    if (m_CurrentFrame.y > m_CurrentAnimation->endFrame.y) {
                        if (m_CurrentAnimation->loop) {
                            m_CurrentFrame = m_CurrentAnimation->startFrame;
                        }
                        else {
                            m_CurrentFrame = m_CurrentAnimation->endFrame;
                        }
                    }
                }

                
                // Clamp the frame indices within the sprite sheet dimensions
                m_CurrentFrame.x = m_CurrentFrame.x, m_CurrentAnimation->endFrame.x;
                m_CurrentFrame.y = m_CurrentFrame.y, m_CurrentAnimation->endFrame.y;

                // Frame update logic here...

            }

            

        }
    }

    void AnimatorComponent::AddAnimation(const Animation& animation) {

        glm::ivec2 currentFrame = GetCurrentFrame();
        float frameDuration = GetFrameDuration();
       // renderer.AddRender(m_Owner, m_Texture, position, m_Scale, m_SpriteSheetDims, currentFrame, frameDuration);

        m_Animations.push_back(animation);
        
    }

    void AnimatorComponent::SetAnimation(const std::string& name) {
        for (auto& animation : m_Animations) {
            if (animation.name == name) {
                m_CurrentAnimation = &animation;
                m_CurrentFrame = animation.startFrame;
                m_FrameTime = 0.0f;
              //  std::cout << "Added Animation: " << animation.name << std::endl;
                break;
            }
        }
    }

    void AnimatorComponent::PlayAnimation(const std::string& name)
    {
        canPlay = true;
        for (auto& animation : m_Animations) {
            if (animation.name == name) {
                m_CurrentAnimation = &animation;
                m_CurrentFrame = animation.startFrame;
                m_FrameTime = 0.0f;
            //    std::cout << "Added Animation: " << animation.name << std::endl;
                break;
            }
        }
    }

    void AnimatorComponent::StopAnimation()
    {
        canPlay = false;
    }

    glm::ivec2 AnimatorComponent::GetCurrentFrame() const {
        return m_CurrentFrame;
    }
    
    float AnimatorComponent::GetFrameDuration() const {
        return m_CurrentAnimation ? m_CurrentAnimation->frameDuration : 0.0f;
    }

    Animation* AnimatorComponent::GetCurrentAnimation()
    {
        return m_CurrentAnimation;
    }

    bool AnimatorComponent::IsAnimated() const {
        return m_CurrentAnimation != nullptr;
    }

    bool AnimatorComponent::canPlayAnimation()
    {
        return canPlay;
    }

    bool AnimatorComponent::IsAnimationPlaying(const std::string& name)
    {
            if (m_CurrentAnimation->name == name) {
                if (m_CurrentAnimation->loop == false && m_CurrentAnimation->endFrame != m_CurrentFrame)
                {
                    return true;
                }
                
            }
            else
            {
                return false;
            }
        
    }

}


