#pragma once

#include "../core/Linalg.h"
#include "../geometry/Triangle.h"

namespace renderer {
class Object {
  public:
    Object();

    void addTriangle(Triangle triangle);
    const std::vector<Triangle>& getTriangles() const;

    void setModelMatrix(const Matrix4& matrix);
    const Matrix4& getModelMatrix() const;

  private:
    std::vector<Triangle> m_triangles;
    Matrix4 m_modelMatrix;
};
} // namespace renderer