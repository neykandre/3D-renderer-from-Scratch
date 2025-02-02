#pragma once

#include "../core/Matrix.h"
#include "../core/Vector.h"

namespace renderer {
class Camera {
  public:
    Camera(const Vector3& position, float fov, float aspect, float near);

    void moveForward(float);
    void moveBackward(float);
    void moveRight(float);
    void moveLeft(float);
    void rotate(float yawOffset, float pitchOffset);

    Matrix4 getProjectionMatrix() const;
    Matrix4 getViewMatrix() const;

    Vector3 getPosition() const;

  private:
    Vector3 m_position;
    Vector3 m_front;
    Vector3 m_right;
    Vector3 m_up;

    float m_yaw;
    float m_pitch;

    float m_fov;
    float m_aspect;
    float m_near;

    void updateCameraVectors();
};
} // namespace renderer