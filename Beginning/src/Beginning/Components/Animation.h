#pragma once

#include <glm.hpp>
#include <string>

namespace Beginning {

    struct Animation {
        std::string name;
        glm::ivec2 startFrame; // The starting frame of the animation (x, y) in the spritesheet grid
        glm::ivec2 endFrame;   // The ending frame of the animation (x, y) in the spritesheet grid
        bool loop;             // Should the animation loop
        float frameDuration;   // Duration of each frame in the animation

        Animation(const std::string& name, const glm::ivec2& startFrame, const glm::ivec2& endFrame, bool loop, float frameDuration)
            : name(name), startFrame(startFrame), endFrame(endFrame), loop(loop), frameDuration(frameDuration) {}

    };

}