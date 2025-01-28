#include "World.h"

namespace renderer {
void World::addObject(std::unique_ptr<Object> object) {
    m_objects.emplace_back(std::move(object));
}

const World::ObjectsArray& World::getObjects() const {
    return m_objects;
}
} // namespace renderer