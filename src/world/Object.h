#pragma once

#include "../geometry/Mesh.h"
#include <memory>

namespace renderer {
class Object {
  public:
    explicit Object(std::unique_ptr<Mesh>);

    void setTransform(Matrix4);
    const Matrix4& getTransform() const;
    void applyTransform(const Matrix4&);

    const Mesh& getRawMesh() const;
    Mesh getTransformedMesh() const;

  private:
    std::unique_ptr<Mesh> m_mesh;
    Matrix4 m_transform;
};
} // namespace renderer