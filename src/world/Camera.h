#pragma once

#include "../core/Linalg.h"

namespace renderer {
class Camera {
    static constexpr Near kDefaultNear = Near{ 0.1f };
    static constexpr Far kDefaultFar   = Far{ 100.f };
    static constexpr Fov kDefaultFov   = Fov{ std::numbers::pi_v<float> / 3.f };

  public:
    Camera() = default;

    Vector3 getGlobalUpVector() const;
    Vector3 calcForwardVector() const;

    Near getNear() const;
    Far getFar() const;
    Fov getFov() const;
    const Matrix4& getViewMatrix() const;

    void moveForward(float distance);
    void moveBack(float distance);
    void moveRight(float distance);
    void moveLeft(float distance);
    void moveUp(float distance);
    void moveDown(float distance);
    void rotateHorizontal(float angle);
    void rotateVertical(float angle);

  private:
    Near m_near          = kDefaultNear;
    Far m_far            = kDefaultFar;
    Fov m_fov            = kDefaultFov;
    Matrix4 m_viewMatrix = kIdentityMatrix4;
};
} // namespace renderer
