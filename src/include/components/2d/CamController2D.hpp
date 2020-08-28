#pragma once

#include "ECS.h"

#include "events/Keyboard.h"
#include "events/Mouse.h"

#include <glm/glm.hpp>

namespace Monarch {

class CamController2D : public Component {
public:
    virtual void update();
    virtual bool onEvent(Event& e);
private:
    float speed = 500;

    //used for panning around the world
    glm::vec2 move = glm::vec2(0, 0);
    glm::vec2 translationVector = glm::vec2(0, 0);

    //TODO: potentially add zoom/rotation

    void calcTranslationVector(float dt);

    bool onCursorMove(MouseMovedEvent& e);
    bool onKeyPressed(KeyPressedEvent& e);
    bool onKeyReleased(KeyReleasedEvent& e);
};

}