#include "components/CamController2D.h"

#include "components/Transform.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace Monarch;
using namespace glm;

void CamController2D::update(){
    static float lastTime = 0;
    float deltaTime = float(glfwGetTime()-lastTime);
    Transform& entityTF = m_entity->getComponent<Transform>();

    calcTranslationVector(deltaTime);
    entityTF.translate(translationVector.x, translationVector.y, 0);

    lastTime = glfwGetTime();
}

bool CamController2D::onEvent(Event& e){
    e.dispatch<KeyPressedEvent>(std::bind(&CamController2D::onKeyPressed, this, std::placeholders::_1));
    e.dispatch<KeyReleasedEvent>(std::bind(&CamController2D::onKeyReleased, this, std::placeholders::_1));
    e.dispatch<MouseMovedEvent>(std::bind(&CamController2D::onCursorMove, this, std::placeholders::_1));
    return false; //should be irrelevant
}

void CamController2D::calcTranslationVector(float dt){
    if(length(move) <= 0){
        translationVector = vec2(0);
        return;
    }
    translationVector = normalize(move) * speed * dt;
}

bool CamController2D::onKeyPressed(KeyPressedEvent& e) {
    if(e.getIsRepeat()) return false;
    switch(e.getKey()){
        case GLFW_KEY_W:
            move += vec2(0, 1);
            break;
        case GLFW_KEY_A:
            move -= vec2(1, 0);
            break;
        case GLFW_KEY_S:
            move -= vec2(0, 1);
            break;
        case GLFW_KEY_D:
            move += vec2(1, 0);
            break;
        default:
            return false;
    }
    return true;
}

bool CamController2D::onKeyReleased(KeyReleasedEvent& e){
    switch(e.getKey()){
        case GLFW_KEY_W:
            move -= vec2(0, 1);
            break;
        case GLFW_KEY_A:
            move += vec2(1, 0);
            break;
        case GLFW_KEY_S:
            move += vec2(0, 1);
            break;
        case GLFW_KEY_D:
            move -= vec2(1, 0);
            break;
        default:
            return false;
    }
    return true;
}

bool CamController2D::onCursorMove(MouseMovedEvent& e) {
    return false;
}