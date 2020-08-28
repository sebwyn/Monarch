#pragma once

#include "ECS.hpp"
#include "TileSheet.hpp"

#include <glm/glm.hpp>

namespace Monarch {

class Sprite : public Component {
public:
    Sprite(Texture& texture);
    //this TileSheet sprite implementation assumes the tile position is static
    //not animated (fair assumption)
    Sprite(TileSheet& tileSheet, int tileX, int tileY);

    virtual void draw();

    inline int getTexId() const { return m_texId; }

    void setTexCoords(glm::vec2 bottomLeft, glm::vec2 topRight){
        m_bottomLeftTexCoord = bottomLeft;
        m_topRightTexCoord = topRight;
    }

    void setColor(glm::vec4 color){ m_color = color; }
private:
    //this assumes the texture ID is static
    unsigned int m_texId;

    glm::vec4 m_color = glm::vec4(1, 1, 1, 1);
    glm::vec2 m_bottomLeftTexCoord = glm::vec2(0, 0);
    glm::vec2 m_topRightTexCoord = glm::vec2(1, 1);
};

}
