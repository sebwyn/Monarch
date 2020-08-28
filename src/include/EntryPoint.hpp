#pragma once

#include "Monarch.h"

using namespace Monarch;

int main(){
    Application* app = createApplication();
    app->run();
    delete app;
    return 0;
}