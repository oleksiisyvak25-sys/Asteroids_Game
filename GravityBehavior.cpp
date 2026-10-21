#include "GravityBehavior.h"
#include "Asteroid.h"
#include "Spaceship.h"
#include "Config.h"

void GravityBehavior::onDestroyed(Asteroid& a) {
    (void)a;
}

void GravityBehavior::applyEffect(Asteroid& a, Spaceship& s, float dt) {
    const float dist = a.getPosition().distanceTo(s.getPosition());
    if (dist >= Config::GRAVITY_RANGE || dist < 1.0f) {
        return;
    }

    const float strength = Config::GRAVITY_STRENGTH * (1.0f - dist / Config::GRAVITY_RANGE);
    const Vector2 pullDir = (a.getPosition() - s.getPosition()).normalized();
    s.addVelocity(pullDir * (strength * dt));
}
