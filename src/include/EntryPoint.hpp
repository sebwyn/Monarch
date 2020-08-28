#pragma once

#include "Monarch.hpp"

using namespace Monarch;

int main(){
    Application* app = createApplication();
    app->run();
    delete app;
    return 0;
}
