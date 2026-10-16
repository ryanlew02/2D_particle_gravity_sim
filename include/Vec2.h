#pragma once
#include <cmath>

struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;

    Vec2() = default;

    Vec2(float x, float y) {
        this->x = x;
        this->y = y;
    }

    Vec2 operator+(const Vec2& other) const {
        return {x + other.x, y + other.y};
    }

    Vec2 operator-(const Vec2& other) const {
        return {x - other.x, y - other.y};
    }

    Vec2 operator*(float scaler) const {
        return {x * scaler, y * scaler};
    }

    Vec2 operator/(float scaler) const {
        return {x / scaler, y / scaler};
    }

    Vec2& operator+=(const Vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vec2& operator-=(const Vec2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vec2& operator*=(const float scaler) {
        x *= scaler;
        y *= scaler;
        return *this;
    }

    Vec2& operator/=(const float scaler) {
        x /= scaler;
        y /= scaler;
        return *this;
    }

    Vec2 operator-() const {
        return {-x, -y};
    }

    [[nodiscard]] float dot(const Vec2& other) const {
        return x * other.x + y * other.y;
    }

    [[nodiscard]] float lengthSquared() const {
        return x * x + y * y;
    }

    [[nodiscard]] float length() const {
        return std::sqrt(lengthSquared());
    }

    [[nodiscard]] Vec2 normalized() const {
        float len = length();

        if (len < 0.0001f) {
            return {0.0f, 0.0f};
        }

        return *this / len;
    }
};
