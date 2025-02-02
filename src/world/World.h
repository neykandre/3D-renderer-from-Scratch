#pragma once

#include "Object.h"
#include <memory>

namespace renderer {
class World {
  public:
    using ObjectsArray = std::vector<std::unique_ptr<Object>>;

    World() = default;

    void addObject(std::unique_ptr<Object>);
    ObjectsArray& getObjects();

  private:
    ObjectsArray m_objects;
};
} // namespace renderer