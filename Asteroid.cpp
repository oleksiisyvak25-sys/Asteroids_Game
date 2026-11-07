#include "Asteroid.h"
#include "AsteroidFactory.h"
#include "Config.h"
#include <algorithm>

Asteroid::Asteroid(int asteroidSize, Vector2 pos, Vector2 vel,
                   std::shared_ptr<AsteroidBehavior> asteroidBehavior)
    : GameObject(pos, vel, static_cast<float>(asteroidSize) * Config::ASTEROID_RADIUS_PER_SIZE),
      size(asteroidSize),
      behavior(std::move(asteroidBehavior)),
      blastRadius(0.0f) {
}

ObjectType Asteroid::getType() const {
    return ObjectType::Asteroid;
}

void Asteroid::onCollision(GameObject& other) {
    if (other.getType() == ObjectType::Projectile) {
        destroy();
    }
}

void Asteroid::onBlast(int dmg) {
    destroy();
}

void Asteroid::destroy() {
    if (!isActive()) {
        return;
    }
    deactivate();
    if (behavior) {
        behavior->onDestroyed(*this);
    }
}

void Asteroid::applyEffect(Spaceship& ship, float dt) {
    if (behavior) {
        behavior->applyEffect(*this, ship, dt);
    }
}

AsteroidType Asteroid::getKind() const {
    return behavior ? behavior->type() : AsteroidType::Normal;
}

std::vector<std::shared_ptr<Asteroid>> Asteroid::split() const {
    std::vector<std::shared_ptr<Asteroid>> fragments;
    if (size <= Config::ASTEROID_MIN_SIZE) {
        return fragments;
    }

    const float currentSpeed = getVelocity().length();
    const float speed = std::max(currentSpeed * 1.2f, Config::FRAGMENT_MIN_SPEED);
    const Vector2 heading = (currentSpeed > 0.01f) ? getVelocity().normalized() : Vector2::fromAngle(getAngle());

    for (int i = 0; i < 2; ++i) {
        const float turn = (i == 0) ? -Config::FRAGMENT_SPREAD : Config::FRAGMENT_SPREAD;
        const Vector2 dir = heading.rotated(turn);
        const Vector2 pos = getPosition() + dir * (getRadius() * 0.5f);

        fragments.push_back(AsteroidFactory::create(getKind(), size - 1, pos, dir * speed));
    }
    return fragments;
}

int Asteroid::getSize() const { return size; }
float Asteroid::getBlastRadius() const { return blastRadius; }