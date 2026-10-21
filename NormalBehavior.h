#ifndef ASTEROIDS_GAME_NORMALBEHAVIOR_H
#define ASTEROIDS_GAME_NORMALBEHAVIOR_H

#include "AsteroidBehavior.h"

class NormalBehavior : public AsteroidBehavior {
public:
    NormalBehavior() = default;
    ~NormalBehavior() override = default;

    void onDestroyed(Asteroid& a) override;
    void applyEffect(Asteroid& a, Spaceship& s, float dt) override;
};

#endif //ASTEROIDS_GAME_NORMALBEHAVIOR_H
