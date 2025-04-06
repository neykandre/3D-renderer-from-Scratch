#include "application/Application.h"

int main() {
    renderer::Application application(renderer::Width(1280), renderer::Height(720),
                                      "3D Renderer");
    application.run();

    return 0;
}