//
// Created by alex on 20.10.26.
//

#ifndef ASTEROIDS_GAME_VECTOR2_H
#define ASTEROIDS_GAME_VECTOR2_H


struct Vector2 {
    float x;
    float y;

    Vector2();
    Vector2(float x, float y);

    Vector2 operator+(const Vector2& o) const;
    Vector2 operator-(const Vector2& o) const;
    Vector2 operator*(float scalar) const;
    float distanceTo(const Vector2& o) const;
};


#endif //ASTEROIDS_GAME_VECTOR2_H
