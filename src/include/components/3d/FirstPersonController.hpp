#pragma once

#include "ECS.hpp"
#include "events/Keyboard.hpp"
#include "events/Mouse.hpp"

#include <glm/glm.hpp>

#define PI 3.141592

namespace Monarch {

//TODO: try to make the first person controller not completely override the entities transform
class FirstPersonController : public Component {
private:
    float speed = 100;
    float mouseSpeed = 0.3;

    glm::vec2 rotate = glm::vec2(0,0);
    glm::vec3 move = glm::vec3(0,0,0);

    glm::vec3 translationVector = glm::vec3(0,0,0);
    glm::vec2 rotationVector = glm::vec2(0, PI);
public:
    FirstPersonController();

    virtual void init();
    virtual void update();
    virtual bool onEvent(Event& e);


private:
    bool onKeyPressed(KeyPressedEvent& e);
    bool onKeyReleased(KeyReleasedEvent& e);
    bool onCursorMove(MouseMovedEvent& e);

    void calcTranslation(float dt);
    void calcRotation(float dt);
};

}
