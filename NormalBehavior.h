//
// Created by alex on 20.10.26.
//

#ifndef ASTEROIDS_GAME_NORMALBEHAVIOR_H
#define ASTEROIDS_GAME_NORMALBEHAVIOR_H

#include "AsteroidBehavior.h"

class NormalBehavior : public AsteroidBehavior {
public:
    NormalBehavior();
    ~NormalBehavior() override = default;

    void onDestroyed(Asteroid* a) override;
    void applyEffect(Asteroid* a, Spaceship* s) override;
};

#endif //ASTEROIDS_GAME_NORMALBEHAVIOR_H
