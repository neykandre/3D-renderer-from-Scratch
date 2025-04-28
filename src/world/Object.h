#pragma once

#include "../core/Linalg.h"
#include "../geometry/Triangle.h"
#include "../light/Material.h"

namespace renderer {
class Object {
  public:
    Object();
    explicit Object(const Material& material);

    void addTriangle(Triangle triangle);
    void setModelMatrix(const Matrix4& matrix);
    void setMaterial(const Material& material);

    const std::vector<Triangle>& getTriangles() const;
    const Matrix4& getModelMatrix() const;
    const Material& getMaterial() const;

  private:
    std::vector<Triangle> m_triangles;
    Matrix4 m_modelMatrix;
    Material m_material;
};
} // namespace renderer