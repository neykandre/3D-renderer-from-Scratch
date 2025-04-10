#include "application/Application.h"
#include "exceptions/Except.h"

#include <iostream>

int main() {
    try {
        renderer::Application application(renderer::Width(1024),
                                          renderer::Height(768), "3D Renderer");
        application.run();

    } catch (...) {
        renderer::react();
    }

    return 0;
}
