#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include "events/Event.hpp"

namespace Monarch {

class KeyEvent : public Event{
public:

    int getKey() const {return key;}

//protected:
    KeyEvent(int _key) : key(_key){}
    int key;
};

class KeyPressedEvent : public KeyEvent {
public:
    KeyPressedEvent(int _key, bool _isRepeat) : KeyEvent(_key), isRepeat(_isRepeat) {};

    static EventType staticType() {return EventType::KeyPressed;}
    virtual EventType getType() const override {return staticType();}

    bool getIsRepeat() const { return isRepeat; }

    virtual std::string debugString() const override{
        std::stringstream ss;
        ss << "Key Pressed: " << key;
        return ss.str();
    }
private:
    bool isRepeat = false;
};

class KeyReleasedEvent : public KeyEvent {
public:
    KeyReleasedEvent(int _key) : KeyEvent(_key) {};

    static EventType staticType() {return EventType::KeyReleased;}
    virtual EventType getType() const override {return staticType();}

    virtual std::string debugString() const override{
        std::stringstream ss;
        ss << "Key Released: " << key;
        return ss.str();
    }
};

}
