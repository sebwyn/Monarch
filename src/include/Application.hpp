#pragma once

#include "Window.hpp"
#include "events/Event.hpp"
#include "events/WindowEvent.hpp"
#include "Layer.hpp"

#include <iostream>
#include <memory>

namespace Monarch {

class Application {
public:

    Application(std::string name, int width, int height);
    virtual ~Application();

    void run();

    void shutdown();

    void onEvent(Event& e);
    bool onWindowClosed(WindowClosedEvent& e);

protected:
    LayerStack layers;

    virtual void update() = 0;
    virtual void handleEvent(Event& e){}

    std::unique_ptr<Window> window;
private:
    bool alive;
};

Application* createApplication();

}
