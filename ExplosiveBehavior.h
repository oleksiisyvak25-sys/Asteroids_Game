//
// Created by alex on 20.10.26.
//

#ifndef ASTEROIDS_GAME_EXPLOSIVEBEHAVIOR_H
#define ASTEROIDS_GAME_EXPLOSIVEBEHAVIOR_H

#include "AsteroidBehavior.h"

class ExplosiveBehavior : public AsteroidBehavior {
public:
    ExplosiveBehavior();
    ~ExplosiveBehavior() override = default;

    void onDestroyed(Asteroid* a) override;
    void applyEffect(Asteroid* a, Spaceship* s) override;
};


#endif //ASTEROIDS_GAME_EXPLOSIVEBEHAVIOR_H
