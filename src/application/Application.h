#pragma once
#include "../world/World.h"

namespace renderer {

class Application {
  public:
    void run();

  private:
    World m_world;
};

} // namespace renderer
