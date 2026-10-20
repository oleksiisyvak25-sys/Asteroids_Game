//
// Created by alex on 20.10.26.
//

#include "Spaceship.h"
#include <cmath>

constexpr float PI = 3.14159265f;

Spaceship::Spaceship()
    : health(3), shield(false) {
    position = Vector2(400.0f, 300.0f);
    radius = 16.0f;
}

ObjectType Spaceship::getType() const {
    return ObjectType::Ship;
}

void Spaceship::rotate(float dir) {
    angle += dir * 5.0f;
}

void Spaceship::thrust() {
    float radians = angle * (PI / 180.0f);
    velocity.x += std::cos(radians) * 20.0f;
    velocity.y += std::sin(radians) * 20.0f;
}

std::shared_ptr<Projectile> Spaceship::shoot(ProjectilePool& pool) {
    float radians = angle * (PI / 180.0f);

    Vector2 nosePos(
        position.x + std::cos(radians) * radius,
        position.y + std::sin(radians) * radius
    );

    Vector2 projVel(
        velocity.x + std::cos(radians) * 350.0f,
        velocity.y + std::sin(radians) * 350.0f
    );

    return pool.acquire(nosePos, projVel);
}

void Spaceship::onCollision(GameObject* other) {
    if (other->getType() == ObjectType::Asteroid) {
        if (!shield) {
            health--;
        }
    }
}
