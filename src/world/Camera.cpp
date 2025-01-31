#include "Camera.h"
#include <cmath>

#define DEG_TO_RAD(x) ((x) * M_PI / 180.0f)

namespace renderer {

Camera::Camera(const Vector3& position, float fov, float aspect, float near,
               float far)
    : m_position(position),
      m_fov(fov),
      m_aspect(aspect),
      m_near(near),
      m_far(far),
      m_yaw(-90.0f),
      m_pitch(0.0f),
      m_up(0, 1, 0) {
    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    m_front.x() = std::cos(DEG_TO_RAD(m_yaw)) * std::cos(DEG_TO_RAD(m_pitch));
    m_front.y() = std::sin(DEG_TO_RAD(m_pitch));
    m_front.z() = std::sin(DEG_TO_RAD(m_yaw)) * std::cos(DEG_TO_RAD(m_pitch));
    m_front.normalize();

    m_right = m_front.cross(Vector3(0, 1, 0)).normalized();

    m_up = m_right.cross(m_front).normalized();
}

void Camera::moveForward(float distance) {
    m_position += m_front * distance;
}

void Camera::moveBackward(float distance) {
    moveForward(-distance);
}

void Camera::moveRight(float distance) {
    m_position += m_right * distance;
}

void Camera::moveLeft(float distance) {
    moveRight(-distance);
}

void Camera::rotate(float yawOffset, float pitchOffset) {
    m_yaw += yawOffset;
    m_pitch += pitchOffset;

    if (m_pitch > 89.0f) {
        m_pitch = 89.0f;
    }
    if (m_pitch < -89.0f) {
        m_pitch = -89.0f;
    }

    updateCameraVectors();
}

Matrix4 Camera::getViewMatrix() const {
    Vector3 forward = (m_position - (m_position + m_front)).normalized();
    Vector3 right   = m_up.cross(forward).normalized();
    Vector3 newUp   = forward.cross(right);

    Matrix4 view = Matrix4::Identity();
    view.block<3, 3>(0, 0) << right, newUp, forward;
    view.block<3, 1>(0, 3) << -right.dot(m_position), -newUp.dot(m_position),
        -forward.dot(m_position);

    return view;    
}

Matrix4 Camera::getProjectionMatrix() const {
    float tanHalfFov   = std::tan(m_fov / 2.0f);
    Matrix4 projection = Matrix4::Zero();

    projection(0, 0) = 1.0f / (m_aspect * tanHalfFov);
    projection(1, 1) = 1.0f / tanHalfFov;
    projection(2, 2) = -(m_far + m_near) / (m_far - m_near);
    projection(2, 3) = -(2.0f * m_far * m_near) / (m_far - m_near);
    projection(3, 2) = -1.0f;

    return projection;
}
} // namespace renderer