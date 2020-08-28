#include "components/FirstPersonController.h"

#include "components/Camera.h"
#include "components/Transform.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace glm;
using namespace Monarch;

FirstPersonController::FirstPersonController() {}

bool FirstPersonController::onKeyPressed(KeyPressedEvent& e) {
    if(e.getIsRepeat()) return false;
    switch(e.getKey()){
        case GLFW_KEY_W:
            move += vec3(0, 0, 1);
            break;
        case GLFW_KEY_A:
            move -= vec3(1, 0, 0);
            break;
        case GLFW_KEY_S:
            move -= vec3(0, 0, 1);
            break;
        case GLFW_KEY_D:
            move += vec3(1, 0, 0);
            break;
        case GLFW_KEY_SPACE:
            move += vec3(0,1,0);
            break;
        case GLFW_KEY_LEFT_SHIFT:
            move -= vec3(0,1,0);
            break;
        default:
            return false;
    }
    return true;
}

bool FirstPersonController::onKeyReleased(KeyReleasedEvent& e){
    switch(e.getKey()){
        case GLFW_KEY_W:
            move -= vec3(0, 0, 1);
            break;
        case GLFW_KEY_A:
            move += vec3(1, 0, 0);
            break;
        case GLFW_KEY_S:
            move += vec3(0, 0, 1);
            break;
        case GLFW_KEY_D:
            move -= vec3(1, 0, 0);
            break;
        case GLFW_KEY_SPACE:
            move -= vec3(0,1,0);
            break;
        case GLFW_KEY_LEFT_SHIFT:
            move += vec3(0,1,0);
            break;
        default:
            return false;
    }
    return true;
}

bool FirstPersonController::onCursorMove(MouseMovedEvent& e) {
    int width = m_entity->getComponent<Camera>().getWidth();
    int height = m_entity->getComponent<Camera>().getHeight();

    rotate = vec2(0,float(width/2-e.getX()));
    rotate += vec2(-float(height/2-e.getY()),0);
    //should we return true?
    return true;
}

void FirstPersonController::calcTranslation(float dt){
    if (length(move) <= 0){ translationVector = vec3(0); return;}

    vec3 forward = m_entity->getComponent<Transform>().getForward();
    vec3 right = m_entity->getComponent<Transform>().getRight();

    translationVector = (move.x*right) + (move.y*vec3(0,1,0)) + (move.z*forward);
    translationVector = normalize(translationVector) * speed * dt;
}

void FirstPersonController::calcRotation(float dt){
    rotationVector += rotate * dt * mouseSpeed;

    if (rotationVector.x > 1.5){
        rotationVector.x = 1.5;
    } else if (rotationVector.x < -1.5){
        rotationVector.x = -1.5;
    }

    rotate =  vec2(0,0);
}


void FirstPersonController::init(){
    update();
}

void FirstPersonController::update(){
    static float lastTime = 0;
    float deltaTime = float(glfwGetTime()-lastTime);
    Transform& entTf = m_entity->getComponent<Transform>();

    calcRotation(deltaTime);
    entTf.setRotation(rotationVector.x,rotationVector.y,0);

    calcTranslation(deltaTime);
    entTf.translate(translationVector.x,translationVector.y,translationVector.z);

    lastTime = glfwGetTime();
}

bool FirstPersonController::onEvent(Event& e){
    e.dispatch<KeyPressedEvent>(std::bind(&FirstPersonController::onKeyPressed, this, std::placeholders::_1));
    e.dispatch<KeyReleasedEvent>(std::bind(&FirstPersonController::onKeyReleased, this, std::placeholders::_1));
    e.dispatch<MouseMovedEvent>(std::bind(&FirstPersonController::onCursorMove, this, std::placeholders::_1));
    return false; //should be irrelevant
}