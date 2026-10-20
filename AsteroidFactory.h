//
// Created by alex on 20.10.26.
//

#ifndef ASTEROIDS_GAME_ASTEROIDFACTORY_H
#define ASTEROIDS_GAME_ASTEROIDFACTORY_H

#include "Vector2.h"
#include <memory>
#include <string>

class Asteroid;

class AsteroidFactory {
public:
    AsteroidFactory() = default;

    static std::shared_ptr<Asteroid> create(const std::string& type, int size, Vector2 pos);
    static std::shared_ptr<Asteroid> create(const std::string& type, int size, Vector2 pos, Vector2 vel);
};


#endif //ASTEROIDS_GAME_ASTEROIDFACTORY_H
