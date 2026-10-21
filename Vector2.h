#ifndef ASTEROIDS_GAME_VECTOR2_H
#define ASTEROIDS_GAME_VECTOR2_H

struct Vector2 {
    float x;
    float y;

    Vector2();
    Vector2(float xValue, float yValue);

    Vector2 operator+(const Vector2& o) const;
    Vector2 operator-(const Vector2& o) const;
    Vector2 operator*(float scalar) const;
    Vector2& operator+=(const Vector2& o);

    float length() const;
    float distanceTo(const Vector2& o) const;
    Vector2 normalized() const;
    Vector2 clampedLength(float maxLength) const;
    Vector2 rotated(float degrees) const;

    static Vector2 fromAngle(float degrees);
};

#endif //ASTEROIDS_GAME_VECTOR2_H
