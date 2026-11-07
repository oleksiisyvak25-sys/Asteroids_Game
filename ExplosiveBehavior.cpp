#include "ExplosiveBehavior.h"
#include "Asteroid.h"
#include "Config.h"

float ExplosiveBehavior::onDestroyed(const Asteroid& a) const {
    return Config::BLAST_BASE_RADIUS + Config::BLAST_RADIUS_PER_SIZE * static_cast<float>(a.getSize());
}

AsteroidType ExplosiveBehavior::type() const {
    return AsteroidType::Explosive;
}

void ExplosiveBehavior::applyEffect(Asteroid& a, Spaceship& s, float dt) {
    (void)a;
    (void)s;
    (void)dt;
}