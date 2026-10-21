#include "NormalBehavior.h"

void NormalBehavior::onDestroyed(Asteroid& a) {
    (void)a;
}

void NormalBehavior::applyEffect(Asteroid& a, Spaceship& s, float dt) {
    (void)a;
    (void)s;
    (void)dt;
}
