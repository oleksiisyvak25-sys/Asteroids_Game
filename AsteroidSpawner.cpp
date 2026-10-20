//
// Created by alex on 20.10.26.
//

#include "AsteroidSpawner.h"
#include "AsteroidFactory.h"
#include <cstdlib>

std::vector<std::shared_ptr<Asteroid>> AsteroidSpawner::spawnWave(int waveNumber) {
    std::vector<std::shared_ptr<Asteroid>> waveAsteroids;

    int count = 3 + waveNumber * 2;

    for (int i = 0; i < count; ++i) {
        float startX = (i % 2 == 0) ? 0.0f : 800.0f;
        float startY = static_cast<float>(rand() % 600);
        float velX = static_cast<float>((rand() % 60) - 30);
        float velY = static_cast<float>((rand() % 60) - 30);

        std::string type = "normal";
        int roll = rand() % 100;
        if (roll < 10 + waveNumber * 5) {
            type = "explosive";
        } else if (roll < 20 + waveNumber * 5) {
            type = "gravity";
        }

        waveAsteroids.push_back(AsteroidFactory::create(type, 3, Vector2(startX, startY), Vector2(velX, velY)));
    }

    return waveAsteroids;
}