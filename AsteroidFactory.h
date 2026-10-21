#ifndef ASTEROIDS_GAME_ASTEROIDFACTORY_H
#define ASTEROIDS_GAME_ASTEROIDFACTORY_H

#include "Vector2.h"
#include "AsteroidType.h"
#include <memory>

class Asteroid;

class AsteroidFactory {
public:
    AsteroidFactory() = default;

    static std::shared_ptr<Asteroid> create(AsteroidType type, int size, Vector2 pos);
    static std::shared_ptr<Asteroid> create(AsteroidType type, int size, Vector2 pos, Vector2 vel);
};

#endif //ASTEROIDS_GAME_ASTEROIDFACTORY_H
