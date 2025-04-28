#include "Object.h"

namespace renderer {
Object::Object()
    : m_modelMatrix(kIdentityMatrix4) {
}

Object::Object(const Material& material)
    : m_modelMatrix(kIdentityMatrix4),
      m_material(material) {
}

void Object::addTriangle(Triangle triangle) {
    m_triangles.emplace_back(std::move(triangle));
}

const std::vector<Triangle>& Object::getTriangles() const {
    return m_triangles;
}

void Object::setModelMatrix(const Matrix4& matrix) {
    m_modelMatrix = matrix;
}

const Matrix4& Object::getModelMatrix() const {
    return m_modelMatrix;
}

void Object::setMaterial(const Material& material) {
    m_material = material;
}

const Material& Object::getMaterial() const {
    return m_material;
}
} // namespace renderer