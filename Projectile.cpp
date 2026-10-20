//
// Created by alex on 20.10.26.
//

#include "Projectile.h"

Projectile::Projectile()
    : lifetime(0.0f), maxLifetime(2.0f) {
    radius = 3.0f;
    isActive = false;
}

ObjectType Projectile::getType() const {
    return ObjectType::Projectile;
}

void Projectile::init(Vector2 pos, Vector2 vel) {
    position = pos;
    velocity = vel;
    lifetime = 0.0f;
    isActive = true;
}

void Projectile::update(float dt) {
    GameObject::update(dt);
    lifetime += dt;
    if (lifetime >= maxLifetime) {
        isActive = false;
    }
}

void Projectile::onCollision(GameObject* other) {
    if (other->getType() == ObjectType::Asteroid) {
        isActive = false;
    }
}