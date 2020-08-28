#pragma once

#include "ECS.hpp"

#include "components/Transform.hpp"

#include "optional.hpp"

#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace Monarch {

/* New Idea for UI 

    -UI Layout System
        


    -hierarchichal UI properties
        -layout (size and position)
            Do all UIElements conform?
            (or is it a property only some have)
        -shown

    -implement a Layout Component
    that when attached to a component will control its
    position and make it conform to the layout

    -any new Layout entities (entities that want to conform to layout)
    will need to implement a calcSize function 

*/

class UIComponent : public Component {
public:

    struct Bounds;

    virtual void draw() final {
        if(!m_parent && shown){
            drawUi();
            for(auto& child : m_children){
                if(child->isShown()) child->drawUi();
            }
        }
    };

    virtual void drawUi(){}

    virtual bool onEvent(Event& e) final {
        if(!m_parent && shown){
            if(onUiEvent()) return true;
            for(auto& child : m_children){
                if(child->isShown() && child->onUiEvent()) return true; 
            }
        }
        return false;
    }

    virtual bool onUiEvent(){ return false; }

    virtual ~UIComponent() override {
        for(UIComponent* child : m_children){
            child->setParent(nullptr);
        }
    }

    void setParent(UIComponent* parent){ m_parent = parent; }
    void addChild(UIComponent* child){
        m_children.emplace_back(child);
        child->setParent(this);
    }

    virtual glm::vec2 calcSize(const glm::vec2& availableSize){ return glm::vec2(0, 0); };

    inline bool isShown() const { return shown; }
    void setShown(bool _shown){ shown = _shown; }

    inline glm::vec2 getDesiredSize(){ return m_desiredSize; }

    //TODO: think about additional calcSize pass
    void layoutSet(glm::vec2 size, glm::vec2 pos){
        size -= glm::vec2(margin.getHorizontal(m_parent->getFinalSize().x), margin.getVertical(m_parent->getFinalSize().y));
        size = calcSize(size);
        //ensure that aspect ratio constraint is still met
        if(aspect.has_value){
            float layoutAspect = size.x / size.y;
            if(!(aspect.value() - 0.01 < layoutAspect && layoutAspect < aspect.value() + 0.01)){
                if(fitToWidth){
                    size.y = size.x * 1.0/aspect.value();
                } else {
                    size.x = size.y * aspect.value();
                }
            }
        }
        m_entity->getComponent<Transform>().setScale(size.x, size.y, 1.0);
        
        glm::vec2 parentFinalSize = m_parent->getFinalSize();
        if(!centered){
            pos += relativePosition.get(parentFinalSize) 
                +  glm::vec2(margin.left.get(parentFinalSize.x), margin.bottom.get(parentFinalSize.y));
        } else {
            pos = m_parent->getFinalPosition() + (parentFinalSize - size)/glm::vec2(2.0);
        }
        m_entity->getComponent<Transform>().setPosition(pos.x, pos.y, 0);
    }

    glm::vec2 getFinalPosition(){ return glm::vec2(m_entity->getComponent<Transform>().getPosition()); }
    glm::vec2 getFinalSize(){ return glm::vec2(m_entity->getComponent<Transform>().getScale()); }

    //these are the base constraints
    class PixelOrNormal {
        public:
            PixelOrNormal(){}
            PixelOrNormal(int pixel){
                isNormal = false;
                m_value = pixel;
            }
            PixelOrNormal(float normal){
                isNormal = true;
                m_value = normal;
            }
            PixelOrNormal(double normal){
                isNormal = true;
                m_value = normal;
            }

            float get(float parentValue){
                if(isNormal) return parentValue * m_value;
                else return m_value;
            }

            const int& operator=(const int& value){
                m_value = value;
                isNormal = false;
                return value;
            }

            const float& operator=(const float& value){
                m_value = value;
                isNormal = true;
                return value;
            }

        private:
            bool isNormal;
            float m_value;
    };

    struct Bounds {
        PixelOrNormal left, right, top, bottom;
        float getHorizontal(float parentWidth){ return left.get(parentWidth) + right.get(parentWidth); }
        float getVertical(float parentHeight){ return top.get(parentHeight) + bottom.get(parentHeight); }
    };
    struct pnVec2 {
        PixelOrNormal x, y;
        glm::vec2 get(glm::vec2 parentValue){ 
            return glm::vec2(x.get(parentValue.x), y.get(parentValue.y)); 
        }
    };

    /*
        Currently Supported constraints

        TODO: These should be moved into some kind of extendable constraint class
    */

    optional<PixelOrNormal> width, height; //used to calc desired
    //used to calc desired and final
    Bounds margin{0, 0, 0, 0}, padding{0, 0, 0, 0};
    //used to calc final
    bool fitToWidth = true;
    optional<float> aspect;

    bool centered = false;
    pnVec2 relativePosition{0, 0};

    /*
        the goal of the measure pass is to try to come up with how much size each element wants
        later this value will be used with other constraints to place and develop a final size
    */
    void measure(const glm::vec2& parentSize){
        //the default margin constraint will be 0
        
        optional<float> pixWidth, pixHeight;
        if(width.has_value){
            pixWidth = width.value().get(parentSize.x);
        }
        if(height.has_value){
            pixHeight = height.value().get(parentSize.y);
        }

        glm::vec2 availableSize = glm::ivec2(
            pixWidth.value_or(parentSize.x) - margin.getHorizontal(parentSize.x) - padding.getHorizontal(parentSize.x),
            pixHeight.value_or(parentSize.y) - margin.getVertical(parentSize.y) - padding.getVertical(parentSize.y)
        );   

        //measure children using availableSize
        for(UIComponent* child : m_children){
            if(child->isShown()){
                child->measure(availableSize);
            }
        }

        glm::vec2 layoutSize = calcSize(availableSize);
        m_desiredSize.x = pixWidth.value_or(layoutSize.x + padding.getHorizontal(parentSize.x));
        m_desiredSize.y = pixHeight.value_or(layoutSize.y + padding.getVertical(parentSize.y));

        if(aspect.has_value && !(width.has_value && height.has_value)){
            if(width.has_value || fitToWidth){
                m_desiredSize.y = m_desiredSize.x * 1.0/aspect.value();
            } else {
                m_desiredSize.x = m_desiredSize.y * aspect.value();
            }
        }

        m_desiredSize += glm::vec2(margin.getHorizontal(parentSize.x), margin.getVertical(parentSize.y));
    }

    /*
        used in arrangement pass
        elements that are shown get formatted by their parents
    */
    void arrange(){
        std::vector<UIComponent*> childrenToFormat;
        childrenToFormat.reserve(m_children.size());
        int numberToFormat = 0;
        for(UIComponent* child : m_children){
            if(child->isShown()){
                childrenToFormat.push_back(child);
                numberToFormat++;
            }
        }
        childrenToFormat.resize(numberToFormat);
        arrangeChildren(childrenToFormat);
        for(UIComponent* child : childrenToFormat){
            child->arrange();
        }
    }

protected:
    virtual void arrangeChildren(const std::vector<UIComponent*>& childrenToFormat){};

    UIComponent* m_parent = nullptr;
    std::vector<UIComponent*> m_children;

    bool shown = true;
    glm::vec2 m_desiredSize = glm::ivec2(0, 0);
};

}
