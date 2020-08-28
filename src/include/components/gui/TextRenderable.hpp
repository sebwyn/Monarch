#pragma once

#include "ECS.hpp"
#include "TrueTypeFont.hpp"
#include "components/gui/UIComponent.hpp"

#include <glm/glm.hpp>
#include <string>

namespace Monarch { 

class TextRenderable : public UIComponent {
public:
    TextRenderable(TrueTypeFont& font);
    
    virtual void drawUi();
    virtual void addChild() final {}

    virtual glm::vec2 calcSize(const glm::vec2& availableSize);

    void setText(std::string text){ m_text = text; }
    void setColor(glm::vec4 color){ m_color = color; }
    void setTextScale(glm::vec2 textScale){ /*m_desiredScale = textScale;*/ }
private:
    TrueTypeFont& m_font;

    std::string m_text = "";
    glm::vec2 m_desiredScale = glm::vec2(1, 1);
    glm::vec2 m_textScale;
    glm::vec4 m_color = glm::vec4(1, 1, 1, 1);
};

}
