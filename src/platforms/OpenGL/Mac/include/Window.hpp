#pragma once

#include <stdio.h>
#include <iostream>
#include <functional>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "events/Event.hpp"

namespace Monarch {

class Window {
public:
    Window(const char* title, int width, int height);
    void setEventCallback(std::function<void(Event&)> func) {data.callback = func;};
    void update() const;
    void clear() const;
    void destroy() const;

    void enableCursor();
    void disableCursor();

    void setSize(int width, int height){ glfwSetWindowSize(window, width, height); } 

    inline int getWidth() const {return data.width;}
    inline int getHeight() const {return data.height;}
    inline bool getCursorEnabled() const { return data.cursorEnabled; }
    ~Window(){destroy();}
private:
    void setGLFWEventCallbacks();

    GLFWwindow* window;
    struct WindowData {
        std::function<void(Event&)> callback;
        int width, height;
        bool cursorEnabled;
    };

    WindowData data;
};

}
