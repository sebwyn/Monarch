#pragma once

#include "Window.h"
#include "events/Event.h"
#include "events/WindowEvent.h"
#include "Layer.h"

#include <iostream>
#include <memory>

namespace Monarch {

class Application {
public:

    Application();
    virtual ~Application();

    void run();

    virtual void update() = 0;
    void shutdown();

    virtual void onEvent(Event& e);
    bool onWindowClosed(WindowClosedEvent& e);

protected:
    LayerStack layers;

    std::unique_ptr<Window> window;
private:
    bool alive;
};

Application* createApplication();

}