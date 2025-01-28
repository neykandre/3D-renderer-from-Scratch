#include "Triangle.h"

namespace renderer {
    Triangle::Triangle(const Vector3& v1, const Vector3& v2, const Vector3& v3,
             const Vector3& n)
        : m_vertices{ v1.homogeneous(), v2.homogeneous(), v3.homogeneous() },
          m_normal{n.x(), n.y(), n.z(), 0} {
            m_normal.normalize();
          };

    void Triangle::transform(const Matrix4& m) {
        for (auto& v : m_vertices) {
            v = m * v;
        }
        m_normal = m * m_normal;
        m_normal.normalize();
    }

    const Triangle::VerticesArray& Triangle::getVertices() const {
        return m_vertices;
    }

    const Vector4& Triangle::getNormal() const {
        return m_normal;
    }
}