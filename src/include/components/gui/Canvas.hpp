#pragma once

#include "components/gui/UIComponent.hpp"

namespace Monarch {

class Canvas : public UIComponent {
public:
    virtual void drawUi() override;

    void setColor(glm::vec4 color){ m_color = color; } 
protected:
    //returns max of all the layed out positions
    virtual glm::vec2 calcSize(const glm::vec2& availableSize) override;
    virtual void arrangeChildren(const std::vector<UIComponent*>& childrenToFormat) override;
private:
    glm::vec4 m_color = glm::vec4(1, 1, 1, 1);
};

}
