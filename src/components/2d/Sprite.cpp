#include "components/Sprite.hpp"

#include "components/Transform.hpp"
#include "Renderer.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

using namespace Monarch;

Sprite::Sprite(Texture& texture) 
 : m_texId(texture.get()) {}

Sprite::Sprite(TileSheet& tileSheet, int tileX, int tileY){
    TileSheet::TexCoords t = tileSheet.getTexCoords(tileX, tileY);
    m_bottomLeftTexCoord = t.bottomLeft;
    m_topRightTexCoord = t.topRight;

    m_texId = tileSheet.getTextureId();
}

void Sprite::draw(){
    if(m_entity->hasComponent<Transform>()){
        Transform& transform = m_entity->getComponent<Transform>();

        Renderer::QuadData data;
        data.position = transform.getPosition();
        data.size = glm::vec2(transform.getScale());
        data.color = m_color;
        data.bottomLeftTexCoords = m_bottomLeftTexCoord;
        data.topRightTexCoords = m_topRightTexCoord;
        data.texID = m_texId;

        Renderer::drawQuad(data);
    }
}
