#include "Projectile.h"
#include "Config.h"
#include <cmath>

Projectile::Projectile()
    : lifetime(0.0f), maxLifetime(Config::PROJECTILE_LIFETIME) {
    radius = Config::PROJECTILE_RADIUS;
    active = false;
}

ObjectType Projectile::getType() const {
    return ObjectType::Projectile;
}

void Projectile::reset() {
    lifetime = 0.0f;
    active = true;
}

void Projectile::init(Vector2 pos, Vector2 vel) {
    reset();
    position = pos;
    velocity = vel;
    angle = std::atan2(vel.y, vel.x) * 180.0f / Config::PI;
}

void Projectile::update(float dt) {
    GameObject::update(dt);
    lifetime += dt;
    if (lifetime >= maxLifetime) {
        active = false;
    }
}

void Projectile::onCollision(GameObject& other) {
    if (other.getType() == ObjectType::Asteroid) {
        active = false;
    }
}

float Projectile::getLifetime() const {
    return lifetime;
}
