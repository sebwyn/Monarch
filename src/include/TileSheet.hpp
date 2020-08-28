#pragma once

#include <glm/glm.hpp>

#include "Texture.hpp"

namespace Monarch {

//currently a TileSheet stores and manages its texture
//Ben doesn't love this
class TileSheet {
public:
    TileSheet(const char* texturePath, int width, int height)
     : m_width(width), m_height(height) {
        m_texture = new Texture(texturePath);
    }

    ~TileSheet(){
        delete m_texture;
    }

    struct TexCoords {
        glm::vec2 bottomLeft;
        glm::vec2 topRight;
    };
    
    unsigned int getTextureId(){
        return m_texture->get();
    }
    TexCoords getTexCoords(int tileX, int tileY){
        return {glm::vec2((float)tileX     * (1.0 / m_width), (float)tileY     * (1.0 / m_height)),
                glm::vec2((float)(tileX+1) * (1.0 / m_width), (float)(tileY+1) * (1.0 / m_height))};
    }

    inline int getWidth() const { return m_width; }
    inline int getHeight() const { return m_height; }
private:
    Texture* m_texture = NULL;
    int m_width, m_height;
};

}
