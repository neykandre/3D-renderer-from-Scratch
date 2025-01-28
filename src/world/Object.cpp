#include "Object.h"

namespace renderer {
Object::Object(std::unique_ptr<Mesh> mesh)
    : m_mesh{ std::move(mesh) },
      m_transform{ Matrix4::Identity() } {};

void Object::setTransform(Matrix4 transform) {
    m_transform = std::move(transform);
}

const Matrix4& Object::getTransform() const {
    return m_transform;
}

void Object::applyTransform(const Matrix4& transform) {
    m_transform *= transform;
}

const Mesh& Object::getRawMesh() const {
    return *m_mesh;
}

Mesh Object::getTransformedMesh() const {
    Mesh transformedMesh = *m_mesh;
    transformedMesh.transform(m_transform);
    return transformedMesh;
}
} // namespace renderer