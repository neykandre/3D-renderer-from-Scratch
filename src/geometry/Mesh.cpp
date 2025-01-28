#include "Mesh.h"

namespace renderer {
    void Mesh::addTriangle(Triangle triangle) {
        m_triangles.emplace_back(std::move(triangle));
    }

    const Mesh::TrianglesArray& Mesh::getTriangles() const {
        return m_triangles;
    }

    void Mesh::transform(const Matrix4& m) {
        for (auto& triangle : m_triangles) {
            triangle.transform(m);
        }
    }
}