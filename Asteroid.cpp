#include "Asteroid.h"
#include "AsteroidFactory.h"
#include "Config.h"
#include <algorithm>

Asteroid::Asteroid(AsteroidType asteroidKind, int asteroidSize, Vector2 pos, Vector2 vel,
                   std::shared_ptr<AsteroidBehavior> asteroidBehavior)
    : size(asteroidSize),
      kind(asteroidKind),
      behavior(std::move(asteroidBehavior)),
      blastRadius(0.0f) {
    position = pos;
    velocity = vel;
    radius = static_cast<float>(size) * Config::ASTEROID_RADIUS_PER_SIZE;
}

ObjectType Asteroid::getType() const {
    return ObjectType::Asteroid;
}

void Asteroid::onCollision(GameObject& other) {
    if (other.getType() == ObjectType::Projectile) {
        destroy();
    }
}

void Asteroid::destroy() {
    if (!active) {
        return;
    }
    active = false;
    if (behavior) {
        behavior->onDestroyed(*this);
    }
}

void Asteroid::applyEffect(Spaceship& ship, float dt) {
    if (behavior) {
        behavior->applyEffect(*this, ship, dt);
    }
}

std::vector<std::shared_ptr<Asteroid>> Asteroid::split() const {
    std::vector<std::shared_ptr<Asteroid>> fragments;
    if (size <= Config::ASTEROID_MIN_SIZE) {
        return fragments;
    }

    const float currentSpeed = velocity.length();
    const float speed = std::max(currentSpeed * 1.2f, Config::FRAGMENT_MIN_SPEED);
    const Vector2 heading = (currentSpeed > 0.01f) ? velocity.normalized() : Vector2::fromAngle(angle);

    for (int i = 0; i < 2; ++i) {
        const float turn = (i == 0) ? -Config::FRAGMENT_SPREAD : Config::FRAGMENT_SPREAD;
        const Vector2 dir = heading.rotated(turn);
        const Vector2 pos = position + dir * (radius * 0.5f);
        fragments.push_back(AsteroidFactory::create(kind, size - 1, pos, dir * speed));
    }
    return fragments;
}

int Asteroid::getSize() const { return size; }
AsteroidType Asteroid::getKind() const { return kind; }
float Asteroid::getBlastRadius() const { return blastRadius; }
void Asteroid::setBlastRadius(float r) { blastRadius = r; }
