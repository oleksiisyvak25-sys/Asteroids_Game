#include "NormalBehavior.h"
#include "Asteroid.h"

float NormalBehavior::onDestroyed(const Asteroid& a) const {
    (void)a;
    return 0.0f;
}

AsteroidType NormalBehavior::type() const {
    return AsteroidType::Normal;
}

void NormalBehavior::applyEffect(Asteroid& a, Spaceship& s, float dt) {
    (void)a;
    (void)s;
    (void)dt;
}