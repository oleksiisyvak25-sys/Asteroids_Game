//
// Created by alex on 20.10.26.
//

#include "Vector2.h"

Vector2::Vector2() {
    x = 0.0f;
    y = 0.0f;
}


Vector2::Vector2(float x, float y) {
    x = x;
    y = y;
}

Vector2 Vector2::operator+(const Vector2& o) const {
    return Vector2(x + o.x, y + o.y);
}

Vector2 Vector2::operator-(const Vector2& o) const {
    return Vector2(x - o.x, y - o.y);
}

Vector2 Vector2::operator*(float scalar) const {
    return Vector2(x * scalar, y * scalar);
}

float Vector2::distanceTo(const Vector2& o) const {
    return std::sqrt((x - o.x) * (x - o.x) + (y - o.y) * (y - o.y));
}
