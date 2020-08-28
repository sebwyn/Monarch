#include "components/gui/TextRenderable.h"

#include "components/Transform.h"
#include "Renderer.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

using namespace Monarch;

TextRenderable::TextRenderable(TrueTypeFont& font)
 : m_font(font) {}

glm::vec2 TextRenderable::calcSize(const glm::vec2& availableSize){
    glm::vec2 textSize = glm::vec2(0, 0);
    for(int i = 0; i < m_text.size(); i++){
        TrueTypeFont::Character character = m_font.getCharacter(m_text[i]);
        textSize.x += character.advance;
        if(i != m_text.size() - 1)
            textSize.x += m_font.getKerning(m_text[i], m_text[i+1]);
        int characterHeight = character.height;
        if(characterHeight > textSize.y){
            textSize.y = characterHeight;
        }
    }

    m_textScale = availableSize / textSize;
    if(m_textScale.x < m_desiredScale.x || m_textScale.y < m_desiredScale.y) {
        if(m_textScale.x < m_textScale.y){
            m_textScale = glm::vec2(m_textScale.x);
        } else {
            m_textScale = glm::vec2(m_textScale.y);
        }
    } else if(m_textScale.x > m_desiredScale.x || m_textScale.y > m_desiredScale.y){
        m_textScale = m_desiredScale;
    }

    std::cout << "Measuring text; scale: " << glm::to_string(textSize * m_textScale) << std::endl;

    return textSize * m_textScale;
}

//this function assumes that calcSize has been run to set m_textScale
void TextRenderable::drawUi(){
    Transform& transform = m_entity->getComponent<Transform>();
    
    glm::vec3 textPos = transform.getPosition();
    glm::vec3 characterPos = textPos;

    //std::cout << "Drawing " << m_text << " at scale: " << glm::to_string(m_textScale) << std::endl;

    for(int i = 0; i < m_text.size(); i++){
        TrueTypeFont::Character character = m_font.getCharacter(m_text[i]);

        characterPos.y = textPos.y - m_textScale.y*(character.height - character.offsetY);

        //std::cout << (char)c << " " << characterPos.x << " " << characterPos.y << " " << std::endl;

        Renderer::QuadData quad;
        quad.position = characterPos;
        quad.size = glm::vec2(character.width, character.height) * m_textScale;
        quad.color = m_color;
        quad.bottomLeftTexCoords = character.bottomLeftTexCoord;
        quad.topRightTexCoords = character.topRightTexCoord;
        quad.texID = m_font.getTexId();

        Renderer::drawQuad(quad);

        if(i != m_text.size() - 1)
            characterPos.x += (character.advance + m_font.getKerning(m_text[i], m_text[i+1])) * m_textScale.x;
    }
}