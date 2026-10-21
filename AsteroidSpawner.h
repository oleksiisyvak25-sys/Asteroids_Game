#ifndef ASTEROIDS_GAME_ASTEROIDSPAWNER_H
#define ASTEROIDS_GAME_ASTEROIDSPAWNER_H

#include "Asteroid.h"
#include <vector>
#include <memory>
#include <random>

class AsteroidSpawner {
private:
    std::mt19937 rng;

    float randomFloat(float lo, float hi);
    int randomInt(int lo, int hi);
    AsteroidType rollType(int waveNumber);

public:
    explicit AsteroidSpawner(unsigned seed = std::random_device{}());

    std::vector<std::shared_ptr<Asteroid>> spawnWave(int waveNumber);

    static int asteroidCountForWave(int waveNumber);
    static int specialChancePercent(int waveNumber);
};

#endif //ASTEROIDS_GAME_ASTEROIDSPAWNER_H
