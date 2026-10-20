//
// Created by alex on 20.10.26.
//

#include "GravityBehavior.h"
#include "Asteroid.h"
#include "Spaceship.h"

GravityBehavior::GravityBehavior() {}

void GravityBehavior::onDestroyed(Asteroid* a) {
    (void)a;
}

void GravityBehavior::applyEffect(Asteroid* a, Spaceship* s) {
    if (!a || !s) return;

    float dist = a->position.distanceTo(s->position);
    if (dist < 200.0f && dist > 1.0f) {
        // Притягуємо корабель до астероїда
        Vector2 pullDir = (a->position - s->position) * (1.0f / dist);
        s->velocity = s->velocity + pullDir * 1.5f;
    }
}