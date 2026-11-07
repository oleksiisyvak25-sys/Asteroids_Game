#ifndef ASTEROIDS_GAME_GRAVITYBEHAVIOR_H
#define ASTEROIDS_GAME_GRAVITYBEHAVIOR_H

#include "AsteroidBehavior.h"

class GravityBehavior : public AsteroidBehavior {
public:
    GravityBehavior() = default;
    ~GravityBehavior() override = default;

    float onDestroyed(const Asteroid& a) const override;
    AsteroidType type() const override;
    void applyEffect(Asteroid& a, Spaceship& s, float dt) override;
};

#endif //ASTEROIDS_GAME_GRAVITYBEHAVIOR_H