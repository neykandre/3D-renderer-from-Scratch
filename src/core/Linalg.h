#pragma once

#include <Eigen/Dense>

namespace renderer {
using Matrix4 = Eigen::Matrix4f;
using Matrix3 = Eigen::Matrix3f;
using Vector4 = Eigen::Vector4f;
using Vector3 = Eigen::Vector3f;

constexpr Matrix3 IdentityMatrix3 = Matrix3::Identity();
constexpr Matrix4 IdentityMatrix4 = Matrix4::Identity();
constexpr Matrix3 ZeroMatrix3     = Matrix3::Zero();
constexpr Matrix4 ZeroMatrix4     = Matrix4::Zero();

inline Matrix4 translate(const Vector3& translation) {
    Eigen::Affine3f transform = Eigen::Affine3f::Identity();
    transform.translate(translation);
    return transform.matrix();
}

inline Matrix4 rotate(const Vector3& axis, float angle) {
    Eigen::Affine3f transform = Eigen::Affine3f::Identity();
    transform.rotate(Eigen::AngleAxisf(angle, axis.normalized()));
    return transform.matrix();
}

inline Matrix4 project(float left, float right, float bottom, float top, float near,
                       float far) {
    Matrix4 matrix = ZeroMatrix4();
    matrix << 2.0f * near / (right - left), 0.0f, (right + left) / (right - left),
        0.0f, 0.0f, 2.0f * near / (top - bottom), (top + bottom) / (top - bottom),
        0.0f, 0.0f, 0.0f, -(far + near) / (far - near),
        -2.0f * far * near / (far - near), 0.0f, 0.0f, -1.0f, 0.0f;
    return matrix;
}
} // namespace renderer