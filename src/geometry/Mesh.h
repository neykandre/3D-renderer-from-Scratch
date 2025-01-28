#pragma once

#include "Triangle.h"
#include <vector>

namespace renderer {
class Mesh {
  public:
    using TrianglesArray = std::vector<Triangle>;

    Mesh() = default;

    void addTriangle(Triangle);
    const TrianglesArray& getTriangles() const;
    void transform(const Matrix4&);

  private:
    TrianglesArray m_triangles;
};
} // namespace renderer