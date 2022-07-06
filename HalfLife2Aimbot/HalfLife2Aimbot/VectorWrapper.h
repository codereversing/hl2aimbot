#pragma once

#include <numbers>

using vec_t = float;

struct QAngle
{
    vec_t x{}, y{}, z{};
    // ...
};

struct Vector
{
public:
    vec_t x{}, y{}, z{};
    // ...
};

float VectorLength(const Vector& vec) {
    return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

void VectorNormalize(Vector& vec)
{
    float magnitude = VectorLength(vec);
    vec.x /= magnitude;
    vec.y /= magnitude;
    vec.z /= magnitude;
}

float VectorDistance(const Vector& first, const Vector& second)
{
    Vector difference = Vector{
        first.x - second.x,
        first.y - second.y,
        first.z - second.z
    };
    return VectorLength(difference);
}

void VectorAngles(const Vector& forward, QAngle& angles)
{
    float tmp{}, yaw{}, pitch{};

    if (forward.y == 0 && forward.x == 0)
    {
        yaw = 0;
        if (forward.z > 0)
            pitch = 270;
        else
            pitch = 90;
    }
    else
    {
        yaw = (atan2(forward.y, forward.x) * 180 / std::numbers::pi);
        if (yaw < 0)
            yaw += 360;

        tmp = sqrt(forward.x * forward.x + forward.y * forward.y);
        pitch = (atan2(-forward.z, tmp) * 180 / std::numbers::pi);
        if (pitch < 0)
            pitch += 360;
    }

    angles.x = pitch;
    angles.y = yaw;
    angles.z = 0;
}
