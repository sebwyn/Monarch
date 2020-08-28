#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include "events/Event.hpp"

namespace Monarch{

class WindowResizedEvent : public Event {
public:
    WindowResizedEvent(int _width, int _height) : width(_width), height(_height) {}

    int getWidth() {return width;}
    int getHeight() {return height;}

    static EventType staticType() {return EventType::WindowResized;}
    virtual EventType getType() const override {return staticType();}

    virtual std::string debugString() const override {
        std::stringstream ss;
        ss << "Window Resized: " << width << "  " << height;
        return ss.str();
    }

private:
    int width, height;
};

class WindowClosedEvent : public Event {
public:
    static EventType staticType() {return EventType::WindowClosed;}
    virtual EventType getType() const override {return staticType();}

    virtual std::string debugString() const override {
        std::stringstream ss;
        ss << "Window Closed";;
        return ss.str();
    }
};

}
