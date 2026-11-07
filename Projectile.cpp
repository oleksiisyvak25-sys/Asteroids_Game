#include "Projectile.h"
#include "Config.h"
#include <cmath>

Projectile::Projectile()
    : GameObject(Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), Config::PROJECTILE_RADIUS),
      lifetime(0.0f), maxLifetime(Config::PROJECTILE_LIFETIME) {
    deactivate();
}

ObjectType Projectile::getType() const {
    return ObjectType::Projectile;
}

void Projectile::reset() {
    lifetime = 0.0f;
    activate();
}

void Projectile::init(Vector2 pos, Vector2 vel) {
    reset();
    setInternalPosition(pos);
    setInternalVelocity(vel);
    setInternalAngle(std::atan2(vel.y, vel.x) * 180.0f / Config::PI);
}

void Projectile::update(float dt) {
    GameObject::update(dt);
    lifetime += dt;
    if (lifetime >= maxLifetime) {
        deactivate();
    }
}

void Projectile::onCollision(GameObject& other) {
    if (other.getType() == ObjectType::Asteroid) {
        deactivate();
    }
}

float Projectile::getLifetime() const {
    return lifetime;
}