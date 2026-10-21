#ifndef ASTEROIDS_GAME_EXPLOSIVEBEHAVIOR_H
#define ASTEROIDS_GAME_EXPLOSIVEBEHAVIOR_H

#include "AsteroidBehavior.h"

class ExplosiveBehavior : public AsteroidBehavior {
public:
    ExplosiveBehavior() = default;
    ~ExplosiveBehavior() override = default;

    void onDestroyed(Asteroid& a) override;
    void applyEffect(Asteroid& a, Spaceship& s, float dt) override;
};

#endif //ASTEROIDS_GAME_EXPLOSIVEBEHAVIOR_H
