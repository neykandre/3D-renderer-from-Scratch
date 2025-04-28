#pragma once
#include <string>

struct Vec3 {
    float x, y, z;
};

enum class CmdType {
    LoadObject,
    AddLight,
    Exit
};

struct Command {
    CmdType type;

    std::string path;
    bool applyTransform = false;
    Vec3 rotAxis{ 0, 0, 0 };
    float rotAngle = 0.f;
    Vec3 translation{ 0, 0, 0 };

    Vec3 lightDir{ 0, 0, 0 };
    Vec3 lightColor{ 0, 0, 0 };
};
