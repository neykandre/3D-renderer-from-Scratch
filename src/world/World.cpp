#include "World.h"

namespace renderer {
void World::addObject(std::unique_ptr<Object> object) {
    m_objects.emplace_back(std::move(object));
}

World::ObjectsArray& World::getObjects() {
    return m_objects;
}
} // namespace renderer