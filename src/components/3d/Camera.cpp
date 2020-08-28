#include "components/Camera.hpp"
#include "components/Transform.hpp"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

using namespace Monarch;

void Camera::setFOV(float fov){
    FOV = fov;

    updateProjectionMatrix();
}

void Camera::updateAspect(float _width, float _height){
    width = _width;
    height = _height;

    updateProjectionMatrix();
}

void Camera::init(){
    //lateUpdate();
    updateProjectionMatrix();
}

void Camera::update(){
    updateLookAt();
    updateViewMatrix();
}

bool Camera::onEvent(Event& e){
    e.dispatch<WindowResizedEvent>(std::bind(&Camera::onWindowResize, this, std::placeholders::_1));
    return false;
}

bool Camera::onWindowResize(WindowResizedEvent& e){
    width = e.getWidth();
    height = e.getHeight();
    updateAspect(width, height);
    return false;
}

void Camera::updateLookAt(){
    glm::vec3 target;
    glm::vec3 position = m_entity->getComponent<Transform>().getPosition();
    switch (lookAtState){
        case lookAtMode::POINT:
            viewDirection = glm::normalize(targetPoint-position);
            break;
        case lookAtMode::ENTITY:
            target = targetEntity->getComponent<Transform>().getPosition();
            viewDirection = glm::normalize(target-position);
            break;
        case lookAtMode::DISABLED:
            viewDirection = m_entity->getComponent<Transform>().getForward();
            break;
    }
}

void Camera::lookAt(Entity* _entity){
    lookAtState = lookAtMode::ENTITY;
    targetEntity = _entity;
    updateLookAt();
}

void Camera::lookAt(float x, float y, float z){
    lookAtState = lookAtMode::POINT;
    targetPoint = glm::vec3(x,y,z);
    updateLookAt();
}

void Camera::disableLookAt(){
    lookAtState = lookAtMode::DISABLED;
}

void Camera::updateViewMatrix(){
    glm::vec3 position = m_entity->getComponent<Transform>().getPosition();
    viewMatrix = glm::lookAt(position,
                             position+viewDirection,
                             glm::vec3(0,1,0));
}

void Camera::updateProjectionMatrix(){
    if (isOrtho) {
        projectionMatrix = glm::ortho(-width/2, width/2,
                                      -height/2, height/2,
                                      1.0f,10000.0f);
    }else{
        projectionMatrix = glm::perspective(glm::radians(FOV),
                                            width / height,
                                            0.1f,
                                            10000.0f);
    }
}

void Camera::ortho(){
    isOrtho = true;
    updateProjectionMatrix();
}

void Camera::perspective(){
    isOrtho = false;
    updateProjectionMatrix();
}
