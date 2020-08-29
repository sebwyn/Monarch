#pragma once

#include "ECS.hpp"

#include "events/Keyboard.hpp"
#include "events/Mouse.hpp"

#include <glm/glm.hpp>

namespace Monarch {

class CamController2D : public Component {
public:
    virtual void init() override;
    virtual void update() override;
    virtual bool onEvent(Event& e) override;
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
