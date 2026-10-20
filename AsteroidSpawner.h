//
// Created by alex on 20.10.26.
//

#ifndef ASTEROIDS_GAME_ASTEROIDSPAWNER_H
#define ASTEROIDS_GAME_ASTEROIDSPAWNER_H

#include "Asteroid.h"
#include <vector>
#include <memory>

class AsteroidSpawner {
public:
    AsteroidSpawner() = default;

    std::vector<std::shared_ptr<Asteroid>> spawnWave(int waveNumber);
};


#endif //ASTEROIDS_GAME_ASTEROIDSPAWNER_H
