#include "Application.h"
#include "Renderer.h"

#include <iostream>

namespace Monarch {

Application::Application(){
    alive = true;
    window = std::unique_ptr<Window>(new Window("Monarch Island", 800, 800));
    window->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));
    Renderer::init();
}

Application::~Application(){
    shutdown();
}

void Application::run(){
    while(alive) {
        window->clear();
        LayerStack::Iterators iterators = layers.getIterators();
        for(auto layer = iterators.begin; layer != iterators.end; layer++){
            (*layer)->update();
        }
        window->update();
    }
}

void Application::shutdown(){
    window->destroy();
    Renderer::destroy();
}

void Application::onEvent(Event& e){
    if(!e.isHandled()){
        e.dispatch<WindowClosedEvent>(std::bind(&Application::onWindowClosed, this, std::placeholders::_1));
        LayerStack::Iterators iterators = layers.getIterators();
        for(auto layer = iterators.rbegin; layer != iterators.rend; layer++){
            if((*layer)->onEvent(e) || e.isHandled()){
                break;
            }
        }
    }
}

bool Application::onWindowClosed(WindowClosedEvent& e){
    alive = false;
    return true;
}
}
