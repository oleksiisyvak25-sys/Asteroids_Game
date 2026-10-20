//
// Created by alex on 20.10.26.
//

#include "ExplosiveBehavior.h"
#include "Asteroid.h"
#include <iostream>

ExplosiveBehavior::ExplosiveBehavior() {}

void ExplosiveBehavior::onDestroyed(Asteroid* a) {
    (void)a;
    std::cout << "[ExplosiveBehavior] Detonated blast radius damage!\n";
}

void ExplosiveBehavior::applyEffect(Asteroid* a, Spaceship* s) {
    (void)a;
    (void)s;
}