#pragma once

#include "ECS.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Monarch {
    class Transform;
}

namespace Monarch {

class Transform : public Component {
private:
    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::quat rotation = glm::vec3(0, 0, 0);
    glm::vec3 size = glm::vec3(1, 1, 1);
public:

    Transform();
    ~Transform();

    void translate(float x, float y, float z);
    void rotate(float x, float y, float z);
    void scale(float x, float y, float z);

    void setPosition(float x, float y, float z);

    //NOTE: angles are in radians
    void setRotation(float x, float y, float z);
    void setRotation(glm::quat _rotation);

	void setScale(float x, float y, float z);


    inline glm::vec3 getPosition() const {return position;}
    inline glm::quat getRotation() const {return rotation;}
    inline glm::vec3 getScale() const {return size;}

    glm::vec3 getForward() const;
    glm::vec3 getRight() const;
    glm::vec3 getUp() const;

    glm::mat4 calcModelMatrix();
};

}

