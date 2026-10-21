#include "Vector2.h"
#include "Config.h"
#include <cmath>

Vector2::Vector2() : x(0.0f), y(0.0f) {}

Vector2::Vector2(float xValue, float yValue) : x(xValue), y(yValue) {}

Vector2 Vector2::operator+(const Vector2& o) const {
    return Vector2(x + o.x, y + o.y);
}

Vector2 Vector2::operator-(const Vector2& o) const {
    return Vector2(x - o.x, y - o.y);
}

Vector2 Vector2::operator*(float scalar) const {
    return Vector2(x * scalar, y * scalar);
}

Vector2& Vector2::operator+=(const Vector2& o) {
    x += o.x;
    y += o.y;
    return *this;
}

float Vector2::length() const {
    return std::sqrt(x * x + y * y);
}

float Vector2::distanceTo(const Vector2& o) const {
    return (*this - o).length();
}

Vector2 Vector2::normalized() const {
    const float len = length();
    if (len < 1e-6f) {
        return Vector2(0.0f, 0.0f);
    }
    return Vector2(x / len, y / len);
}

Vector2 Vector2::clampedLength(float maxLength) const {
    const float len = length();
    if (len <= maxLength || len < 1e-6f) {
        return *this;
    }
    return *this * (maxLength / len);
}

Vector2 Vector2::rotated(float degrees) const {
    const float rad = degrees * Config::PI / 180.0f;
    const float c = std::cos(rad);
    const float s = std::sin(rad);
    return Vector2(x * c - y * s, x * s + y * c);
}

Vector2 Vector2::fromAngle(float degrees) {
    return Vector2(1.0f, 0.0f).rotated(degrees);
}
