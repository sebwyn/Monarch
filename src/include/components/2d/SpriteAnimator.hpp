#pragma once

#include "ECS.hpp"
#include "TileSheet.hpp"

#include <chrono>
#include <map>

namespace Monarch {

class SpriteAnimator : public Component {
public:

    struct Animation {
        TileSheet* tileSheet;
        glm::ivec2 startFrame = {0,0};
        int numFrames = 1;
        float playbackSpeed = 0.25;
        bool mirrored = false;
        bool loop = false;
    };

    virtual void update();

    void start();
    void stop() {m_playing = false;}

    void addAnimation(const char* name, Animation animation);
    void setAnimation(const char* animation);
private:
    void reset(){
        m_lastStart = std::chrono::high_resolution_clock::now();
        m_currentIndexX = m_animation.startFrame.x;
        m_currentIndexY = m_animation.startFrame.y;
        //std::cout << "reset: " << m_animation.startFrame.x << " : " << m_animation.startFrame.y << std::endl;
        m_frameCount = 0;
    }

    std::map<const char*,Animation> m_animations;
    Animation m_animation;

    std::chrono::high_resolution_clock::time_point m_lastStart;
    int m_currentIndexX, m_currentIndexY, m_frameCount;

    bool m_playing;

};

}
