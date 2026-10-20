//
// Created by alex on 20.10.26.
//

#include "NormalBehavior.h"

NormalBehavior::NormalBehavior() {}

void NormalBehavior::onDestroyed(Asteroid* a) {
    (void)a;
}

void NormalBehavior::applyEffect(Asteroid* a, Spaceship* s) {
    (void)a;
    (void)s;
}