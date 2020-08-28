#include "components/gui/Canvas.h"

#include "Renderer.h"

using namespace Monarch;

void Canvas::drawUi(){
    
    Renderer::QuadData quad;
    quad.position = m_entity->getComponent<Transform>().getPosition();
    quad.size = glm::vec2(m_entity->getComponent<Transform>().getScale());
    quad.color = m_color;
    quad.bottomLeftTexCoords = glm::vec2(0, 0);
    quad.topRightTexCoords = glm::vec2(1, 1);
    quad.texID = Renderer::getBlankTexture();
    
    Renderer::drawQuad(quad);
}

glm::vec2 Canvas::calcSize(const glm::vec2& availableSize){
    glm::vec2 greatest = glm::ivec2(0, 0);
    for(UIComponent* child : m_children){
        //child will be formatted
        glm::ivec2 endpoint = child->relativePosition.get(availableSize) + child->getDesiredSize();
        if(endpoint.x > greatest.x){
            greatest.x = endpoint.x;
        }
        if(endpoint.y > greatest.y){
            greatest.y = endpoint.y;
        }
    }
    return greatest;
}

void Canvas::arrangeChildren(const std::vector<UIComponent*>& childrenToFormat){
    glm::vec2 myPos = getFinalPosition();
    if(m_parent){
        const glm::vec2 parentFinalSize = m_parent->getFinalSize();
        myPos += glm::vec2(margin.left.get(parentFinalSize.x), margin.bottom.get(parentFinalSize.y));
    }
    for(UIComponent* child : m_children){
        glm::ivec2 finalPos = myPos;
        child->layoutSet(child->getDesiredSize(), myPos);
    }
}