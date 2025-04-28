#include "Vertex.h"

namespace renderer {
Vertex::Vertex(const Vector3& position, const Vector3& normal)
    : m_position{ position.x(), position.y(), position.z(), 1.0f },
      m_normal{ normal.x(), normal.y(), normal.z(), 0.0f } {
    m_normal.normalize();
}

Vertex::Vertex(const Vector4& position, const Vector4& normal)
    : m_position{ position },
      m_normal{ normal } {
    m_normal.normalize();
}

const Vector4& Vertex::getPosition() const {
    return m_position;
}

const Vector4& Vertex::getNormal() const {
    return m_normal;
}

} // namespace renderer