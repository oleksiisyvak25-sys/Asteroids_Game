#ifndef ASTEROIDS_GAME_ASTEROIDBEHAVIOR_H
#define ASTEROIDS_GAME_ASTEROIDBEHAVIOR_H

class Asteroid;
class Spaceship;

class AsteroidBehavior {
public:
    virtual ~AsteroidBehavior() = default;

    virtual void onDestroyed(Asteroid& a) = 0;
    virtual void applyEffect(Asteroid& a, Spaceship& s, float dt) = 0;
};

#endif //ASTEROIDS_GAME_ASTEROIDBEHAVIOR_H
