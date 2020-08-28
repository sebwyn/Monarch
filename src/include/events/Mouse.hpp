#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include "events/Event.h"

namespace Monarch{

class MouseButtonEvent : public Event{
public:
    int getButton() const {return button;}
protected:
    MouseButtonEvent(int _button) : button(_button) {}
    int button;
};

class MouseButtonPressedEvent : public MouseButtonEvent {
public:
    MouseButtonPressedEvent(int _button) : MouseButtonEvent(_button) {}

    static EventType staticType() {return EventType::MouseButtonPressed;}
    virtual EventType getType() const override {return staticType();}

    virtual std::string debugString() const override {
        std::stringstream ss;
        ss << "Mouse Clicked: " << button;
        return ss.str();
    }
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
public:
    MouseButtonReleasedEvent(int _button) : MouseButtonEvent(_button) {}

    static EventType staticType() {return EventType::MouseButtonReleased;}
    virtual EventType getType() const override {return staticType();}

    virtual std::string debugString() const override {
        std::stringstream ss;
        ss << "Mouse Released: " << button;
        return ss.str();
    }
};

class MouseMovedEvent : public Event {
public:
    MouseMovedEvent(double _x, double _y) : x(_x), y(_y) {}

    double getX() const {return x;}
    double getY() const {return y;}

    static EventType staticType() {return EventType::MouseMoved;}
    virtual EventType getType() const override {return staticType();}

    virtual std::string debugString() const override {
        std::stringstream ss;
        ss << "Mouse Moved: " << x << "  " << y;
        return ss.str();
    }
private:
    double x,y;
};

class MouseScrolledEvent : public Event {
public:
    MouseScrolledEvent(double x, double y) : xOffset(x), yOffset(y) {}

    double getXOffset() const {return xOffset;}
    double getYOffset() const {return yOffset;}

    static EventType staticType() {return EventType::MouseScrolled;}
    virtual EventType getType() const override {return staticType();}

    virtual std::string debugString() const override {
        std::stringstream ss;
        ss << "Mouse Scrolled: " << xOffset << "  " << yOffset;
        return ss.str();
    }
private:
    double xOffset, yOffset;
};

}