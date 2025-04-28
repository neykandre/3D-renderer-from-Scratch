#include "Triangle.h"

namespace renderer {
Triangle::Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3)
    : m_vertices{ v1, v2, v3 } {
}

const std::array<Vertex, 3>& Triangle::getVertices() const {
    return m_vertices;
}
} // namespace renderer