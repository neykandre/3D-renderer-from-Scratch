#pragma once

#include "Object.h"

namespace renderer {

class World {
  public:
    World() = default;

    void addObject(Object object);
    const std::vector<Object>& getObjects() const;

  private:
    std::vector<Object> m_objects;
};

} // namespace renderer