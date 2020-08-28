#include "components/SpriteAnimator.h"

#include "components/Sprite.h"

//temporarily use chrono for timing
#include <chrono>

using namespace Monarch;

void SpriteAnimator::update(){
    if (!m_playing) return;
    auto now = std::chrono::high_resolution_clock::now();
    float animationTime = (std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastStart)).count() / 1000.0;
    if(animationTime > m_frameCount * m_animation.playbackSpeed){
        if(m_frameCount >= m_animation.numFrames){
            if (m_animation.loop){
                reset();
            }else{
                m_playing = false;
                return;
            }
        }
        
        TileSheet::TexCoords tc = m_animation.tileSheet->getTexCoords(m_currentIndexX, m_currentIndexY);
        if (m_animation.mirrored) {
            m_entity->getComponent<Sprite>().setTexCoords({tc.topRight.x,tc.bottomLeft.y},
                                                        {tc.bottomLeft.x, tc.topRight.y});
        }else{
            m_entity->getComponent<Sprite>().setTexCoords(tc.bottomLeft, tc.topRight);
        }
        m_frameCount++;

        if (++m_currentIndexX == m_animation.tileSheet->getWidth()) {
            m_currentIndexX = 0;
            m_currentIndexY--;
        }
            
    }
}

void SpriteAnimator::start() {
    m_playing = true;
    m_currentIndexX = m_animation.startFrame.x;
    m_currentIndexY = m_animation.startFrame.y;
}

void SpriteAnimator::addAnimation(const char* name, Animation animation){
    m_animations[name] = animation;
}

void SpriteAnimator::setAnimation(const char* name){
    m_animation = m_animations[name];
    m_playing = false;
    //std::cout << "initializing animation: " << m_animation.startFrame.x << " : " << m_animation.startFrame.y << std::endl;
    reset();
}