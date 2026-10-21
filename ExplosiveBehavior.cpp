#include "ExplosiveBehavior.h"
#include "Asteroid.h"
#include "Config.h"

void ExplosiveBehavior::onDestroyed(Asteroid& a) {
    a.setBlastRadius(Config::BLAST_BASE_RADIUS + Config::BLAST_RADIUS_PER_SIZE * static_cast<float>(a.getSize()));
}

void ExplosiveBehavior::applyEffect(Asteroid& a, Spaceship& s, float dt) {
    (void)a;
    (void)s;
    (void)dt;
}
