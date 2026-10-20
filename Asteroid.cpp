//
// Created by alex on 20.10.26.
//

#include "Asteroid.h"
#include "AsteroidFactory.h"

Asteroid::Asteroid(int size, Vector2 pos, Vector2 vel, std::shared_ptr<AsteroidBehavior> behavior)
    : size(size), behavior(std::move(behavior)) {
    position = pos;
    velocity = vel;
    radius = static_cast<float>(size) * 14.0f;
}

ObjectType Asteroid::getType() const {
    return ObjectType::Asteroid;
}

void Asteroid::onCollision(GameObject* other) {
    if (other->getType() == ObjectType::Projectile) {
        isActive = false;
        if (behavior) {
            behavior->onDestroyed(this);
        }
    }
}

std::vector<std::shared_ptr<Asteroid>> Asteroid::split() {
    std::vector<std::shared_ptr<Asteroid>> fragments;

    if (size > 1) {
        Vector2 vel1(velocity.y, -velocity.x);
        Vector2 vel2(-velocity.y, velocity.x);

        fragments.push_back(AsteroidFactory::create("normal", size - 1, position, vel1));
        fragments.push_back(AsteroidFactory::create("normal", size - 1, position, vel2));
    }

    return fragments;
}