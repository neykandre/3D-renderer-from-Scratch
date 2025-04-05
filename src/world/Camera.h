#pragma once

#include "../core/Linalg.h"

namespace renderer {
class Camera {
    static constexpr Near kDefaultNear   = Near{ 0.1f };
    static constexpr Far kDefaultFar     = Far{ 100.f };
    static constexpr Fov kDefaultFov     = Fov{ std::numbers::pi_v<float> / 3.f };
    static constexpr float kDefaultSpeed = 0.05f;

  public:
    Camera() = default;

    Near getNear() const;
    Far getFar() const;
    Fov getFov() const;
    Matrix4 getViewMatrix() const;

    void move(const Vector3& movDir);

  private:
    Near m_near          = kDefaultNear;
    Far m_far            = kDefaultFar;
    Fov m_fov            = kDefaultFov;
    Matrix4 m_viewMatrix = kIdentityMatrix4;
};
} // namespace renderer