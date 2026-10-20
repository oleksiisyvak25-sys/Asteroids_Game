//
// Created by alex on 20.10.26.
//

#ifndef ASTEROIDS_GAME_ASTEROIDBEHAVIOR_H
#define ASTEROIDS_GAME_ASTEROIDBEHAVIOR_H

class Asteroid;
class Spaceship;

class AsteroidBehavior {
public:
    virtual ~AsteroidBehavior() = default;
    virtual void onDestroyed(Asteroid* a) = 0;
    virtual void applyEffect(Asteroid* a, Spaceship* s) = 0;
};


#endif //ASTEROIDS_GAME_ASTEROIDBEHAVIOR_H
