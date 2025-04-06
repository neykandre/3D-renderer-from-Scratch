#pragma once

#include <Eigen/Dense>

namespace renderer {
using Matrix4 = Eigen::Matrix4f;
using Matrix3 = Eigen::Matrix3f;
using Vector4 = Eigen::Vector4f;
using Vector3 = Eigen::Vector3f;

const Matrix3 kIdentityMatrix3 = Matrix3::Identity();
const Matrix4 kIdentityMatrix4 = Matrix4::Identity();
const Matrix3 kZeroMatrix3     = Matrix3::Zero();
const Matrix4 kZeroMatrix4     = Matrix4::Zero();

inline Matrix4 makeTranslationMatrix(const Vector3& translation) {
    Eigen::Affine3f transform = Eigen::Affine3f::Identity();
    transform.translate(translation);
    return transform.matrix();
}

inline Matrix4 makeRotationMatrix(const Vector3& axis, float angle) {
    Eigen::Affine3f transform = Eigen::Affine3f::Identity();
    transform.rotate(Eigen::AngleAxisf(angle, axis.normalized()));
    return transform.matrix();
}

struct Near {
    float value;
    constexpr explicit Near(float value)
        : value(value) {
    }
};

struct Far {
    float value;
    constexpr explicit Far(float value)
        : value(value) {
    }
};

struct Fov {
    float value;
    constexpr explicit Fov(float value)
        : value(value) {
    }
};

inline Matrix4 makeProjectionMatrix(Near near, Far far, Fov fov, float aspect) {
    float top    = near.value * std::tan(fov.value / 2.f);
    float bottom = -top;
    float right  = top * aspect;
    float left   = -right;

    Matrix4 matrix = kZeroMatrix4;
    matrix << 2.0f * near.value / (right - left), 0.0f,
        (right + left) / (right - left), 0.0f, 0.0f,
        2.0f * near.value / (top - bottom), (top + bottom) / (top - bottom), 0.0f,
        0.0f, 0.0f, -(far.value + near.value) / (far.value - near.value),
        -2.0f * far.value * near.value / (far.value - near.value), 0.0f, 0.0f, -1.0f,
        0.0f;
    return matrix;
}
} // namespace renderer