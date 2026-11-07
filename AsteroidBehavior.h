#ifndef ASTEROIDS_GAME_ASTEROIDBEHAVIOR_H
#define ASTEROIDS_GAME_ASTEROIDBEHAVIOR_H

#include "AsteroidType.h"

class Asteroid;
class Spaceship;

class AsteroidBehavior {
public:
    virtual ~AsteroidBehavior() = default;

    virtual float onDestroyed(const Asteroid& a) const = 0;

    virtual AsteroidType type() const = 0;

    virtual void applyEffect(Asteroid& a, Spaceship& s, float dt) = 0;
};

#endif //ASTEROIDS_GAME_ASTEROIDBEHAVIOR_H