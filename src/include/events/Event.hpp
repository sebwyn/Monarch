#pragma once

#include <functional>
#include <string>

namespace Monarch{

enum class EventType { 
    KeyPressed, KeyReleased,
    MouseMoved, MouseButtonPressed, MouseButtonReleased, MouseScrolled,
    WindowResized, WindowClosed
};

class Event{
public:
    virtual EventType getType() const = 0;
    virtual std::string debugString() const = 0;

    template<typename T, typename F>
    bool dispatch(const F& callback){
        if (getType() == T::staticType()){
            handled = callback(*(T*)this);
            return true;
        }
        return false;
    }

    inline bool isHandled() const {return handled;}
private:
    bool handled = false;
};

}