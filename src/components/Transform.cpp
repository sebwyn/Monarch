#include "components/Transform.h"

#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <iostream>

using namespace Monarch;

Transform::Transform() = default;

Transform::~Transform(){
}

void Transform::translate(float x, float y, float z){
    position += glm::vec3(x, y, z);
}

void Transform::rotate(float x, float y, float z){
    rotation *= glm::quat(glm::vec3(x, y, z));
    rotation = glm::normalize(rotation);
}

void Transform::scale(float x, float y, float z){
    size *= glm::vec3(x, y, z);
}

void Transform::setPosition(float x, float y, float z){
    position = glm::vec3(x, y, z);
}

void Transform::setRotation(float x, float y, float z){
    rotation = glm::quat(glm::vec3(x, y, z));
}

void Transform::setRotation(glm::quat _rotation){
    rotation = _rotation;
}

void Transform::setScale(float x, float y, float z){
    size = glm::vec3(x, y, z);
}

glm::mat4 Transform::calcModelMatrix(){
    glm::mat4 modelMatrix = glm::mat4(1.0);
    glm::mat4 transMat  = glm::translate(glm::mat4(1.0), position);
    glm::mat4 rotMat = glm::mat4_cast(rotation);
    glm::mat4 scaleMat = glm::scale(glm::mat4(1.0), size);
    modelMatrix = transMat * rotMat * scaleMat;

    return modelMatrix;
}

glm::vec3 Transform::getForward() const{
    glm::mat4 rMat = glm::mat4_cast(rotation);
    return glm::vec3(rMat*glm::vec4(0,0,1,0));
}

glm::vec3 Transform::getRight() const{
    glm::mat4 rMat = glm::mat4_cast(rotation);
    return glm::vec3(rMat*glm::vec4(-1,0,0,0));
}

glm::vec3 Transform::getUp() const{
    glm::mat4 rMat = glm::mat4_cast(rotation);
    return glm::vec3(rMat*glm::vec4(0, 1, 0, 0));
}
