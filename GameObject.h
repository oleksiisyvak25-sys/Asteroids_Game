//
// Created by alex on 20.10.26.
//

#ifndef ASTEROIDS_GAME_GAMEOBJECT_H
#define ASTEROIDS_GAME_GAMEOBJECT_H

#include "Vector2.h"

enum class ObjectType {
    Ship,
    Asteroid,
    Projectile
};

class GameObject {
public:
    Vector2 position;
    Vector2 velocity;
    float angle;
    float radius;
    bool isActive;

    GameObject();
    virtual ~GameObject() = default;

    virtual ObjectType getType() const = 0;
    virtual void update(float dt);
    virtual void onCollision(GameObject* other) = 0;

    void wrapAround(float screenWidth, float screenHeight);
};


#endif //ASTEROIDS_GAME_GAMEOBJECT_H
