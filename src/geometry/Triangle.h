#pragma once

#include "../core/Matrix.h"
#include "../core/Vector.h"
#include <array>

namespace renderer {
class Triangle {
  public:
    using VerticesArray = std::array<Vector4, 3>;

    Triangle(const Vector3& v1, const Vector3& v2, const Vector3& v3,
             const Vector3& n);

    void transform(const Matrix4&);
    const VerticesArray& getVertices() const;
    const Vector4& getNormal() const;

  private:
    VerticesArray m_vertices;
    Vector4 m_normal;
};
} // namespace renderer