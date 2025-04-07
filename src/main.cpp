#include "application/Application.h"
#include "exceptions/Except.h"

int main() {
    try {

        renderer::Application application(renderer::Width(800),
                                          renderer::Height(600), "3D Renderer");
        application.run();
    } catch (...) {
        renderer::react();
    }

    return 0;
}