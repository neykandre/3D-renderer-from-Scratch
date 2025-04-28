#pragma once

#include "../core/Linalg.h"

namespace renderer {
class Vertex {
  public:
    Vertex() = default;
    Vertex(const Vector3& position, const Vector3& normal);
    Vertex(const Vector4& position, const Vector4& normal);

    const Vector4& getPosition() const;
    const Vector4& getNormal() const;

  private:
    Vector4 m_position;
    Vector4 m_normal;
};
} // namespace renderer