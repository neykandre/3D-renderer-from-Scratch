#include "World.h"

namespace renderer {
void World::addObject(Object object) {
    m_objects.emplace_back(std::move(object));
}

const std::vector<Object>& World::getObjects() const {
    return m_objects;
}
} // namespace renderer