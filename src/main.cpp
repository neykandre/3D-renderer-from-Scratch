#include "application/Application.h"

int main() {
    renderer::Application application(renderer::Width(800), renderer::Height(600),
                                      "3D Renderer");
    application.run();

    return 0;
}